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
};

using DT = unordered_map<int, unordered_map<int, char>>;

DT data;

int max_y = numeric_limits<int>::min();
int max_x = numeric_limits<int>::min();
int min_x = numeric_limits<int>::max();
int min_y = numeric_limits<int>::max();

char getDataAt(DT & data, Point p)
{
    if (p.y > max_y) return ' ';
    auto xData = data.find(p.x);
    if (xData == end(data))
        return '.';
    auto yData = xData->second.find(p.y);
    if (yData == end(xData->second))
        return '.';
    
    return yData->second;
}

void setDataAt(Point p, char value)
{
    data[p.x][p.y] = value;
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

bool IsFlowingWater(Point location)
{
    return getDataAt(data, location) == '|';
}

bool IsClayAt(Point location)
{
    return getDataAt(data, location) == '#';
}

bool IsSandAt(Point location)
{
    return getDataAt(data, location) == '.';
}

bool IsSettledWaterAt(Point location)
{
    return getDataAt(data, location) == '~';
}

void PrintData()
{
    ofstream iOutFile("out.txt");
    int count = 0;
    for (int y = 1; y <= max_y; ++y)
    {
        for (int x = min_x - 10; x <= max_x + 10; ++x)
        {
            char d = getDataAt(data, {x, y});
            if (( d == '~') && y >= min_y && y <= max_y)
                count++;
            
            if (d == '.')
                d = ' ';
            iOutFile << d;
        }
        iOutFile << endl;
    }
    
    iOutFile << "WATER COUNT: " << count << endl;
    cout << count;
    iOutFile.close();
}

void Fill(Point p, int direction)
{
    if (IsFlowingWater(p))
    {
        if (direction > 0)
            Fill(p.Right(), direction);
        else if (direction < 0)
            Fill(p.Left(), direction);
        
        setDataAt(p, '~');
    }
}

bool Flow(Point p, int direction = 0)
{
    if (!IsSandAt(p))
        return !IsClayAt(p) && !IsSettledWaterAt(p);
    
    setDataAt(p, '|');
    
    bool canFlowFurther = Flow(p.Down(), 0);
    if (canFlowFurther)
        return true;
    
    canFlowFurther = (direction <= 0) && Flow(p.Left(), -1);
    canFlowFurther |= (direction >= 0) && Flow(p.Right(), 1);
    
    if (canFlowFurther)
    {
        return true;
    }
    
    if (direction == 0)
    {
        Fill(p, -1);
        Fill(p.Right(), 1);
    }
    return false;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string line;
    ifstream iFile;
    bool demo = false;
    
    iFile.open(demo ? "day17d.txt" : "day17r.txt");
    
    Point spring{ 500, 0 };
    
    while (getline(iFile, line))
    {
        auto tokens = regexMatch(line, R"((\S)=(\d+), \S=(\d+)\.\.(\d+))");
        
        bool xLeads = tokens[0] == "x";
        
        int veinLead = stoi(tokens[1]);
        int veinMinorA = stoi(tokens[2]);
        int veinMinorB = stoi(tokens[3]);
        
        if (xLeads)
        {
            for (int minor = veinMinorA; minor <= veinMinorB; ++minor)
            {
                setDataAt({veinLead, minor}, '#');
                if (minor > max_y)
                    max_y = minor;
                if (minor < min_y)
                    min_y = minor;
            }
            if (veinLead < min_x)
                min_x = veinLead;
            if (veinLead > max_x)
                max_x = veinLead;
        }
        else
        {
            for (int minor = veinMinorA; minor <= veinMinorB; ++minor)
            {
                setDataAt({minor, veinLead}, '#');
                if (minor > max_x)
                    max_x = minor;
                if (minor < min_x)
                    min_x = minor;
            }
            if (veinLead > max_y)
                max_y = veinLead;
            if (veinLead < min_y)
                min_y = veinLead;
        }
    }
    
    Flow(spring);
    PrintData();
    
    return 0;
}
