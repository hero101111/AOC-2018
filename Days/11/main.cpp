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
#include <algorithm>
#include <queue>
#include <deque>
#include <stack>
#include <numeric>
#include <list>
#include <tgmath.h>

using namespace std;

struct Point
{
    int x,y;
    
    bool operator ==(const Point & other) const
    {
        return x == other.x && y == other.y;
    }
    
    bool operator < (const Point & other) const
    {
        if (x < other.x) return true;
        if (x == other.x && y < other.y)
            return true;
        return false;
    }
};

using DT = map<Point, int>;

int GetPower(int x, int y, int gridSerialNumber)
{
    int rackId = x + 10;
    int ret = rackId * y;
    ret += gridSerialNumber;
    
    ret *= rackId;
    ret = ret % 1000;
    ret = floor(ret / 100);
    ret -= 5;
    return ret;
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


int main(int argc, const char * argv[]) {
    // insert code here...
   // ifstream f;
   // f.open("inputR.txt");
   //string s;
    
    
    //while (getline(f, s))
    //{
        //vector<string> v =
        //regexMatch(s, R"(position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>)");
    //}
    
    //out << GetPower({101, 153}, 71) << endl;
    int serialNumber = 5235;
   /* DT points;
    for (int i = 1; i <= 300; ++i)
    {
        for (int j = 1; j <= 300; ++j)
        {
            points[Point{i, j}] = GetPower(Point{i, j}, serialNumber);
        }
    }*/
    int data[301][301] = {0};
    for (int i = 1; i <= 300; ++i)
    {
        for (int j = 1; j <= 300; ++j)
        {
            data[i][j] = GetPower(i, j, serialNumber);
        }
    }
    
    int maxX = numeric_limits<int>::min();
    int maxY = numeric_limits<int>::min();
    long long maxValue = numeric_limits<int>::min();
    int maxSq = numeric_limits<int>::min();
    for (int i = 1; i <= 299; ++i)
        for (int j = 1; j <= 299; ++j)
        {
            long long old_sum = data[i][j];
            int marginX = i;
            int marginY = j;
            while (++marginX <= 300 && ++marginY <= 300)
            {
                long long t = old_sum;
                for (int j2 = j; j2 <= marginY; ++j2)
                    t += data[marginX][j2];
                for (int i2 = i; i2 < marginX; ++i2)
                    t += data[i2][marginY];
                if (t > maxValue)
                {
                    maxValue = t;
                    maxX = i;
                    maxY = j;
                    maxSq = marginX - i;
                }
                old_sum = t;
            }
            
            
        }
    while (true)
    {
        int i = 0; ++i;
    }
    cout << maxValue << endl << maxX << " " << maxY << endl << maxSq << endl;
    return 0;
}
