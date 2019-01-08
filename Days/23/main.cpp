// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//
//  main.cpp
//  aoc1
//
//  Created by Gabriel on 01/12/2018.
//  Copyright � 2018 Gabriel. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <algorithm>
#include <queue>
#include <deque>
#include <stack>
#include <numeric>
#include <list>
#include <tgmath.h>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Point
{
	int x, y, z;

	bool operator ==(const Point & other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator != (const Point & other) const
	{
		return !operator ==(other);
	}

	bool operator < (const Point & other) const
	{
		if (other.y < y) return true;
		if (y == other.y && x < other.x)
			return true;
		if (y == other.y && x == other.x && z < other.z)
			return true;
		return false;
	}

	Point Down() const
	{
		return Point{ x, y + 1 };
	}

	Point Left() const
	{
		return Point{ x - 1, y };
	}

	Point Right() const
	{
		return Point{ x + 1, y };
	}

	Point Up() const
	{
		return Point{ x, y - 1 };
	}

	Point FromOrientation(char orientation) const
	{
		switch (orientation)
		{
		case 'N':
			return Up();
		case 'E':
			return Right();
		case 'W':
			return Left();
		case 'S':
			return Down();
		}
		return *this;
	}

	int ManDist(const Point & p)  const
	{
		return abs(x - p.x) + abs(y - p.y) + abs(z - p.z);
	}
};

using DT = unordered_map<int, unordered_map<int, char>>;

DT mapData;

int max_y = numeric_limits<int>::min();
int max_x = numeric_limits<int>::min();
int min_x = numeric_limits<int>::max();
int min_y = numeric_limits<int>::max();
int min_z = numeric_limits<int>::max();
int max_z = numeric_limits<int>::min();

static const char kDoorX = '|';
static const char kDoorY = '-';
static const char kWall = '#';
static const char kRoom = '.';
static const char kStart = 'X';
static const char kUnknown = '?';

char getDataAt(DT & data, Point p)
{
	auto xData = data.find(p.x);
	if (xData == end(data))
		return kWall;
	auto yData = xData->second.find(p.y);
	if (yData == end(xData->second))
		return kWall;

	return yData->second;
}

void setDataAt(Point p, char value)
{
	mapData[p.x][p.y] = value;
}

vector<string> regexMatch(string s, string regex)
{
	std::regex re2(regex);
	std::smatch match2;
	std::regex_search(s, match2, re2);
	vector<string> ret;
	for (int i = 1; i <= match2.size(); ++i)
		ret.push_back(match2.str(i));
	return ret;
}

bool IsRoomAt(Point location)
{
	return getDataAt(mapData, location) == kRoom;
}

bool IsDoorAt(Point location)
{
	auto d = getDataAt(mapData, location);
	return d == kDoorX || d == kDoorY;
}

bool IsWall(Point location)
{
	return getDataAt(mapData, location) == kWall;
}

void GenerateData(const string & inputData)
{
	Point currentRoom;

	stack<Point> branches;

	for (char c : inputData)
	{
		if (c == '^')
		{
			currentRoom = { 0, 0 };
			setDataAt(currentRoom, kStart);
		}
		else if (c == '$')
		{
			// end
		}
		else if (c == '(')
		{
			branches.push(currentRoom);
		}
		else if (c == ')')
		{
			branches.pop();
		}
		else if (c == '|')
		{
			currentRoom = branches.top();
		}
		else
		{
			Point newDoor = currentRoom.FromOrientation(c);
			Point newRoom = newDoor.FromOrientation(c);

			setDataAt(newDoor, (c == 'E' || c == 'W') ? kDoorX : kDoorY);
			setDataAt(newRoom, kRoom);

			currentRoom = newRoom;
		}
	}
}

template <>
struct hash<Point>
{
	std::size_t operator()(const Point& k) const
	{
		return k.y * 10000 + k.x;
	}
};

void GetNeighbours(const Point & p, vector<Point> & ret)
{
	ret.clear();
	Point east = p.FromOrientation('E');
	Point west = p.FromOrientation('W');
	Point north = p.FromOrientation('N');
	Point south = p.FromOrientation('S');

	if (!IsWall(east)) ret.push_back(east);
	if (!IsWall(west)) ret.push_back(west);
	if (!IsWall(north)) ret.push_back(north);
	if (!IsWall(south)) ret.push_back(south);
}

unordered_map<Point, int> dist;
unordered_map<Point, Point> prevPoint;

struct MinQueuePred
{
	bool operator()(const Point & a, const Point & b) const
	{
		int da = dist[a];
		int db = dist[b];
		bool bb = da < db;
		if (bb)
			return true;
		if (da == db)
			return a < b;

		return bb;
	}
};

void FillPaths(const Point & from)
{
	queue<Point> Q;
	unordered_map<Point, int> visitData;

	Q.push(from);
	visitData[from] = 0;

	vector<Point> neighbours;
	while (!Q.empty())
	{
		Point curr = Q.front();
		Q.pop();
		int currD = visitData[curr];
		GetNeighbours(curr, neighbours);
		for (auto & n : neighbours)
		{
			if (!IsWall(n) && visitData.find(n) == end(visitData))
			{
				visitData[n] = currD + (IsDoorAt(n) ? 1 : 0);
				Q.push(n);
			}
		}
	}

	auto maxE = max_element(begin(visitData), end(visitData), [](auto & e1, auto & e2) { return e1.second < e2.second; });
	cout << maxE->second << endl;

	auto acc = count_if(begin(visitData), end(visitData), [&](auto & e) { return IsRoomAt(e.first) && e.second >= 1000; });

	cout << acc << endl;
}

void  RunPaths(const Point & from)
{
	multiset<Point, MinQueuePred> Q;
	vector<Point> neigh;
	Q.clear();

	for (int x = min_x; x <= max_x; ++x)
	{
		for (int y = min_y; y <= max_y; ++y)
		{
			Point p{ x, y };
			dist[p] = numeric_limits<int>::max() - 1;
			if (x == from.x && y == from.y)
				dist[p] = 0;
			prevPoint[p] = { numeric_limits<int>::max(), numeric_limits<int>::max() };
			Q.insert(p);
		}
	}

	while (!Q.empty())
	{
		Point u = *Q.begin();
		Q.erase(Q.begin());

		GetNeighbours(u, neigh);

		for (auto v : neigh)
		{
			int alt = dist[u] + 1;
			if (alt < dist[v])
			{
				dist[v] = alt;
				prevPoint[v] = u;

				Q.erase(v);
				Q.insert(v);
			}
		}
	}
}

void BoundData()
{
	for (auto & xData : mapData)
	{
		int x = xData.first;
		if (x < min_x)
			min_x = x;
		if (x > max_x)
			max_x = x;

		for (auto & yData : xData.second)
		{
			int y = yData.first;
			if (y < min_y)
				min_y = y;
			if (y > max_y)
				max_y = y;
		}
	}
	min_x -= 1;
	max_x += 1;
	min_y -= 1;
	max_y += 1;
}

void Detect()
{
	int maxPathCount = numeric_limits<int>::min();

	int thCount = 0;

	Point start{ 0, 0 };

	RunPaths(start);

	for (int x = min_x; x <= max_x; ++x)
	{
		for (int y = min_y; y <= max_y; ++y)
		{
			Point p{ x, y };

			if (IsRoomAt(p))
			{
				int doorCount = 0;
				while (p.x != numeric_limits<int>::max() || p == start)
				{
					if (IsDoorAt(p)) ++doorCount;
					//    ret.push_back(u);
					p = prevPoint[p];
				}
				if (doorCount > maxPathCount)
					maxPathCount = doorCount;
				if (doorCount >= 1000)
					thCount++;

			}
		}
	}

	cout << "MAX PATH: " << maxPathCount << endl;
	cout << "TH COUNT: " << thCount << endl;
}

void PrintData()
{
	ofstream iOutFile("out.txt");

	int count = 0;
	for (int y = min_y; y <= max_y; ++y)
	{
		for (int x = min_x; x <= max_x; ++x)
		{
			char d = getDataAt(mapData, { x, y });

			iOutFile << d;
		}
		iOutFile << endl;
	}

	iOutFile << "COUNT: " << count << endl;
	iOutFile.close();
}

struct Bot
{
	Point location;
	int radius;

	bool operator ==(const Bot & b) const {
		return location == b.location && radius == b.radius;
	}
};

int inRange(vector<Bot> & bots, Point p)
{
	int count = 0;
	for (auto & b : bots)
	{
		//if (&b == &*maxIt)
		//   continue;

		if (b.location.ManDist(p) <= b.radius)
			count++;
	}
	return count;
}

int main(int argc, const char * argv[]) {

	// insert code here...
	string line;
	ifstream iFile;
	bool demo = false;

	iFile.open(demo ? "c:\\aoc\\day20d.txt" : "c:\\aoc\\day23r.txt");

	vector<Bot> bots;
	string inputData;


	while (getline(iFile, inputData))
	{
		// pos=<91216303,39592740,32677318>, r=96168637
		auto tokens = regexMatch(inputData, R"(pos=<(-?\d+),(-?\d+),(-?\d+)>,\sr=(\d+))");
		Bot bot;
		bot.location.x = stoi(tokens[0]);
		bot.location.y = stoi(tokens[1]);
		bot.location.z = stoi(tokens[2]);
		bot.radius = stoi(tokens[3]);
		bots.push_back(bot);

		if (bot.location.x < min_x) min_x = bot.location.x;
		if (bot.location.x > max_x) max_x = bot.location.x;
		if (bot.location.y < min_y) min_y = bot.location.y;
		if (bot.location.y > max_y) max_y = bot.location.y;
		if (bot.location.z < min_z) min_z = bot.location.z;
		if (bot.location.z > max_z) max_z = bot.location.z;
	}


	//auto maxIt = max_element(begin(bots), end(bots), [](auto & b1, auto & b2){return b1.radius < b2.radius;});
	//cout << maxIt->radius << endl;


   // cout << max_x - min_x << endl;
	//cout << max_y - min_y << endl;
	//cout << max_z - min_z << endl;

	//cout << inRange(bots, *maxIt) << endl;


	/*
	int offset = -300000;

	int maxCount = numeric_limits<int>::min();
	int minManDist = 0;
	Point minP;
	for (int z = 0 - offset; z != 0 + offset; ++z)
	{
		for (int y = 0 - offset; y != 0 + offset; ++y)
		{
			for (int x = 0 - offset; x != 0 + offset; ++x)
			{
				int xxxx = inRange(bots, {x, y, z});
				cout << minManDist << " " << maxCount << "      | C: " << xxxx <<  endl;
				if (xxxx > maxCount)
				{
					maxCount = xxxx;
					minManDist = Point{0, 0, 0}.ManDist({x, y, z});
					minP = {x, y, z};

					cout << minP.x << " " << minP.y << " " << minP.z << endl;
				}
				else if (xxxx == maxCount)
				{
					minManDist = std::min(minManDist, Point{0, 0, 0}.ManDist({x, y, z}));
				}
			}
		}

	}*/

	auto maxBot = max_element(begin(bots), end(bots), [&](auto & b1, auto & b2) { return inRange(bots, b1.location) < inRange(bots, b2.location); });

	cout << maxBot->location.x << " " << maxBot->location.y << " " << maxBot->location.z << " " << endl;

	int maxRange = 0; //inRange(bots, maxBot->location);


	/*int startx = maxBot->location.x;//24574374; //mine
	int starty = maxBot->location.y;//40663946;
	int startz = maxBot->location.z;//57352823;*/


	int startx = maxBot->location.x;
	int starty = maxBot->location.y;
	int startz = maxBot->location.z;


	//int startx = 23700389; //mine
	//int starty = 42553861;
	//int startz = 58368752;

	Point p = { startx, starty, startz };
	maxRange = inRange(bots, p);

	Point origin{ 0, 0, 0 };
	int minManDist = p.ManDist(origin);

	cout << "INITIAL MDIST: " << minManDist << endl;
	cout << "R: " << maxRange << endl;


	/*while (true) {
	int offsetx = 200;
	int offsety = 200;
	int offsetz = 200;
		int step = 1;
		bool broke = false;
	for (int x = -offsetx; x <= offsetx; x += step)
	{
		Point p2{p.x + x, p.y, p.z};
		int newRange = inRange(bots, p2);
		if (newRange < maxRange)
			break;
		for (int y = -offsety; y <= offsety; y +=step)
		{
			Point p2{p.x + x, p.y + y, p.z};
			int newRange = inRange(bots, p2);
			if (newRange < maxRange)
				break;

			for (int z = -offsetz; z <= offsetz; z += step)
			{
				Point p2{p.x + x, p.y + y, p.z + z};
				int newRange = inRange(bots, p2);
				if (newRange > maxRange)
				{
					cout << "NEW RANGE: " << newRange << endl;
					maxRange = newRange;

					minManDist = p2.ManDist(origin);
					cout << p2.x << " " << p2.y << " " << p2.z << " " << endl;
					p = p2;
					cout << endl;
					broke = true;
					break;
				}

				if (newRange == maxRange)
				{
					int newManhDist = p2.ManDist(origin);
					if (newManhDist < minManDist)
					{
						minManDist = newManhDist;
						cout << "NEW MANH: " << newManhDist << endl;
						cout << "RANGE: " << maxRange << endl;
						cout << p2.x << " " << p2.y << " " << p2.z << " " << endl;
						p = p2;
						broke = true;
						break;
					}
				}
				else
				{
				  //  cout << ".";
					break;
				}

			}
			if (broke) break;
		}
		if (broke) break;
	}
	}*/

	vector<vector<int>> mappings =
	{
		 { -1, 0, 1}
		, { -1, 1, 0}
		, { -1, -1, 0}
		, { -1, 0, -1}

		, { 0, 1, 1}
		, { 0, 1, -1}
		, { 0, -1, -1}
		, { 0, -1, 1}

		,  { 1, 0, 0 }
		, { -1, 0, 0}
		, { 0, 1,  0}
		, { 0, -1, 0}
		, { 0, 0,  1}
		, { 0, 0, -1}

		, { 1, 1, 0 }
		, { 1, -1, 0}
		, { 1, 0, -1}
		, { 1, 0, 1 }



		, { 1, -1, 1}
		, { 1, 1, -1}
		, { 1, -1, 1}
		, { 1, -1, -1}
		, { -1, -1, -1}
	};

	int step = 1;
	int strategy = 0;
	while (true)
	{
		//p.x += step;
		//p.x -= step;
		//p.y += step;
		//p.y -= step;
		//p.z += step;
		//p.z -= step;

		strategy = (strategy + 1) % mappings.size();
		cout << strategy << endl;
		int strategyCost = 0;
		Point p2 = p;
		while (true)
		{
			strategyCost++;
			if (strategyCost > 50'00'000)
				break;

			p2.x += mappings[strategy][0];
			p2.y += mappings[strategy][1];
			p2.z += mappings[strategy][2];

			int newRange = inRange(bots, p2);
			if (newRange > maxRange)
			{
				cout << "NEW RANGE: " << newRange << endl;
				maxRange = newRange;
				cout << p2.x << " " << p2.y << " " << p2.z << " " << endl;
				cout << endl;
				strategy = 0;
				p = p2;
				//break;
			}
		}
		//int newManhDist = p.ManDist(origin);
		//if (newManhDist < minManDist)
		//    minManDist = newManhDist;
	}


	cout << "MIN: " << minManDist << endl;



	//cout << minP.x << " " << minP.y << " " << minP.z << endl;
   // cout << minManDist << endl;

	return 0;
}
