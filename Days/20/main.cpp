// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
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
	int x, y;

	bool operator ==(const Point & other) const
	{
		return x == other.x && y == other.y;
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
};

using DT = unordered_map<int, unordered_map<int, char>>;

DT mapData;

int max_y = numeric_limits<int>::min();
int max_x = numeric_limits<int>::min();
int min_x = numeric_limits<int>::max();
int min_y = numeric_limits<int>::max();

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
    
    auto maxE = max_element(begin(visitData), end(visitData),[](auto & e1, auto & e2){ return e1.second < e2.second; });
    cout << maxE->second << endl;
    
    auto acc = count_if(begin(visitData), end(visitData), [&](auto & e){ return IsRoomAt(e.first) && e.second >= 1000;});
    
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
					//	ret.push_back(u);
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

int main(int argc, const char * argv[]) {

	// insert code here...
	string line;
	ifstream iFile;
	bool demo = false;

	iFile.open(demo ? "day20d.txt" : "day20r.txt");

	string inputData;
	getline(iFile, inputData);

	GenerateData(inputData);
	BoundData();

	//PrintData();

	//Detect();
    
    FillPaths({0, 0});

	

	return 0;
}
