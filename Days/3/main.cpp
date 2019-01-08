// day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//
//  main.cpp
//  aoc1
//
//  Created by Gabriel on 01/12/2018.
//  Copyright © 2018 Gabriel. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <regex>
#include <set>

using namespace std;

struct Point
{
	int x, y;

	bool operator < (const Point & other) const
	{
		if (x < other.x) return true;
		if (y < other.y) return true;
		return false;
	}

	bool operator == (const Point & other) const
	{
		return x == other.x && y == other.y;
	}
};

struct Block
{
	int left, top, width, height;

	bool operator ==(const Block & other) const
	{
		return left == other.left && top == other.top && width == other.width && height == other.height;
	}

	bool isWithin(Point p) const
	{
		return p.x >= left && p.x < left + width && p.y >= top && p.y < top + height;
	}
};

int main(int argc, const char * argv[]) {
	// insert code here...
	ifstream f;
	f.open(R"(C:\aoc\day3\Debug\day3.txt)");
	string s;
	set<Point> intersectionPoints;
	vector<Block> blocks;

	int minWidth = numeric_limits<int>::max();
	int maxWidth = numeric_limits<int>::min();
	int minHeight = numeric_limits<int>::max();
	int maxHeight = numeric_limits<int>::min();
	while (getline(f, s))
	{
		//cout << s;
		std::string result;
		std::regex re(R"(@\s(\d+),(\d+):\s(\d+)x(\d+))");
		std::smatch match;
		std::regex_search(s, match, re);

		Block b;
		b.left = stoi(match.str(1));
		b.top = stoi(match.str(2));
		b.width = stoi(match.str(3));
		b.height = stoi(match.str(4));

		if ((b.left) < minWidth) minWidth = b.left;
		if (b.left + b.width > maxWidth) maxWidth = b.left + b.width;
		if (b.top < minHeight) minHeight = b.top;
		if (b.top + b.height > maxHeight) maxHeight = b.top + b.height;

		blocks.push_back(b);
	}

	for (int i = minWidth; i <= maxWidth; ++i)
	{
		for (int j = minHeight; j <= maxHeight; ++j)
		{
			int intersectCount = 0;
			for (auto & block : blocks)
			{
				if (block.isWithin(Point{ i, j }))
					intersectCount++;
				if (intersectCount >= 2) break;
			}

			if (intersectCount >= 2)
				intersectionPoints.insert(Point{ i, j });
		}
	}

	cout << intersectionPoints.size() << endl;

	return 0;
}
