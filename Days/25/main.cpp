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
    int x, y, z, t;
    
    bool operator ==(const Point & other) const
    {
        return x == other.x && y == other.y && z == other.z && other.t == t;
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
        if (y == other.y && x == other.x && z == other.z && t < other.t)
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
        return abs(x - p.x) + abs(y - p.y) + abs(z - p.z) + abs(t - p.t);
    }
};

string lowerstring(string s)
{
    string newS = s;
    transform(begin(newS), end(newS), begin(newS), ::tolower);
    return newS;
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
    assert(!"to be implemented");
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

void PrintData()
{
    ofstream iOutFile("out25.txt");
    
    int count = 0;
    
    iOutFile << "COUNT: " << count << endl;
    iOutFile.close();
}

using DT = vector<Point>;

DT data;



void ReadData(string inputFile)
{
    string mainRegex = R"((-?\d+),(-?\d+),(-?\d+),(-?\d+))";
    
    ifstream immuneFile(inputFile);
    string readLine;
    while (getline(immuneFile, readLine))
    {
        readLine = lowerstring(readLine);
        auto tokens = regexMatch(readLine, mainRegex);
        Point p;
        p.x = stoi(tokens[0]);
        p.y = stoi(tokens[1]);
        p.z = stoi(tokens[2]);
        p.t = stoi(tokens[3]);
        data.push_back(p);
    }
}

size_t getCount()
{
    return 0;
    
    //  count_if(begin(), end(), [&](auto & g){ return ; });
   
    /*sort(begin(), end(), [&](auto & e1, auto e2)
    {
        return e1 < e2;
    });*/
}

int main(int argc, const char * argv[]) {
    
    // insert code here...
    string line;
    bool demo = false;
    
    if (demo)
    {
        ReadData("day25d.txt");
    }
    else
    {
        ReadData("day25r.txt");
    }
    
    vector<vector<Point>> constelations;
    
    while (!data.empty())
    {
        Point p = data.front();
        data.erase(data.begin());
        
        if (constelations.empty())
        {
            vector<Point> c1 { p };
            constelations.insert(begin(constelations), c1);
            continue;
        }
        
        bool found = false;
        for (auto & c : constelations)
        {
            for (auto & op : c)
            {
                if (p.ManDist(op) <= 3)
                {
                    c.push_back(p);
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        
        if (!found)
        {
            vector<Point> c1 { p };
            constelations.insert(begin(constelations), c1);
            continue;
        }
    }
    
    bool busy = true;
    while (busy)
    {
        busy = false;
        for (auto & c1 : constelations)
        {
                for (auto & c2 : constelations)
                {
                    if (&c1 == &c2)
                        continue;
                    
                    for (auto p1 : c1)
                    {
                        for (auto p2 : c2)
                        {
                                if (p1.ManDist(p2) <= 3)
                                {
                                    c1.push_back(p2);
                                    c2.erase(remove(begin(c2), end(c2), p2));
                                    busy = true;
                                    break;
                                }
                            if (busy) break;
                        }
                        if (busy) break;
                    }
                    if (busy) break;
                }
            if (busy) break;
        }
        constelations.erase(remove_if(begin(constelations), end(constelations), [](auto & c){ return c.size() == 0;}), end(constelations));
    }
    
    cout << constelations.size();
    
    return 0;
}
