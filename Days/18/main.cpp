// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifdef _MSCVER
#include "pch.h"
#endif
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
        return ! operator ==(other);
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
    
    vector<Point> GetNeighbours() const
    {
        vector<Point> ret;
        ret.push_back(Left());
        ret.push_back(Up());
        ret.push_back(Right());
        ret.push_back(Down());
        
        ret.push_back(Up().Left());
        ret.push_back(Up().Right());
        ret.push_back(Down().Left());
        ret.push_back(Down().Right());
        
        return ret;
    }

};

using DT = char[50][50];

DT data{0};
DT clone{0};

int max_y = 49; //numeric_limits<int>::min();
int max_x = 49; //numeric_limits<int>::min();
int min_x = 0; //numeric_limits<int>::max();
int min_y = 0; //numeric_limits<int>::max();


map<Point, vector<Point>> neighCache;


bool PointIsValid(const Point p)
{
    return p.x >= min_x && p.x <= max_x && p.y >= min_y && p.y <= max_y;
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

void PrintData()
{
    ofstream iOutFile("out.txt");
    
    for (int line = 0; line <= max_y; ++line)
    {
        for (int x = 0; x <= max_x; ++x)
        {
            iOutFile << data[x][line];
        }
        iOutFile << endl;
    }
    
    iOutFile.close();
}

void Tick()
{
    for (int x = 0; x <= max_x; ++x)
    {
        for (int y = 0; y <= max_y; ++y)
        {
            clone[x][y] = data[x][y];
        }
    }
    
    for (int x = 0; x <= max_x; ++x)
    {
        for (int y = 0; y <= max_y; ++y)
        {
            char acre = data[x][y];
            int neighLumber = 0;
            int neighWood = 0;
            int neighOpen = 0;
            
            const auto & neighPoints = neighCache[{x, y}];
            
            for (const Point & p : neighPoints)
            {
                if (PointIsValid(p))
                {
                    char d = data[p.x][p.y];
                    switch (d)
                    {
                        case '.':
                            ++neighOpen;
                            break;
                        case '|':
                            ++neighWood;
                            break;
                        case '#':
                            ++neighLumber;
                            break;
                    }
                }
            }
            
            if (acre == '.') // open
            {
                if (neighWood >= 3)
                    clone[x][y] = '|'; // becomes tree
            }
            else if (acre == '|') // tree
            {
                if (neighLumber >= 3)
                    clone[x][y] = '#'; // becomes lumber
            }
            else if (acre == '#') // lumber
            {
                if (neighWood >= 1 && neighLumber >= 1)
                {
                    
                }
                else
                {
                    clone[x][y] = '.'; // becomes open
                }
            }
        }
    }
    
    for (int x = 0; x <= max_x; ++x)
    {
        for (int y = 0; y <= max_y; ++y)
        {
            data[x][y] = clone[x][y];
        }
    }
}

int GetValue()
{
    int lumber = 0;
    int wood = 0;
    for (int x = 0; x <= max_x; ++x)
    {
        for (int y = 0; y <= max_y; ++y)
        {
            char acre = data[x][y];
            if (acre == '#') lumber++;
            if (acre == '|') wood++;
        }
    }
    return lumber * wood;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string line;
    ifstream iFile;
    bool demo = false;
    
    iFile.open(demo ? "day18d.txt" : "day18r.txt");
    
    for (int x = 0; x <= max_x; ++x)
        for (int y = 0; y <= max_y; ++y)
        {
            neighCache[{x, y}] = Point{x, y}.GetNeighbours();
        }
    
    int y = 0;
    while (getline(iFile, line))
    {
        //auto tokens = regexMatch(line, R"((\S)=(\d+), \S=(\d+)\.\.(\d+))");
        for (int i = 0; i <= max_x; ++i)
        {
            data[i][y] = line[i];
        }
        y++;
    }
    
    ofstream dO("out.txt");
    for (long long i = 0; i < 10000; ++i)
         {
        Tick();
             dO << i + 1 << " : " << GetValue() << endl;
             dO.flush();
         }
    
  //  PrintData();
    
    cout << GetValue() << endl;
    
    return 0;
}
