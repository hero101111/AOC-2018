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

struct Star
{
    Point location;
    Point velocity;
    
    bool operator == (const Star & s) const
    {
        return location == s.location && velocity == s.velocity;
    }
    
    bool operator < (const Star & s) const
    {
        return location < s.location;
    }
};

using DT = vector<Star>;

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

void Render(const DT & data, int step)
{
    if (data.size() == 0)
        return;
    
    auto xminmax = minmax_element(begin(data), end(data), [](auto & a1, auto & a2) { return a1.location.x < a2.location.x; });
    auto yminmax = minmax_element(begin(data), end(data), [](auto & a1, auto & a2) { return a1.location.y < a2.location.y; });
    
    int w = xminmax.second->location.x - xminmax.first->location.x;
    int h = yminmax.second->location.y - yminmax.first->location.y;
    if (w > 500 || h > 500)
        return;
    
    static string outFilePath = "outD/";
    outFilePath += "f";
    ofstream outF(outFilePath + ".txt");
    
    outF << step << endl;
    
    set<Point> positions;
    for (auto d : data)
    {
        positions.insert({d.location.x, d.location.y});
    }
    
    for (int j = yminmax.first->location.y; j <= yminmax.second->location.y; ++j)
    {
        for (int i = xminmax.first->location.x; i <= xminmax.second->location.x; ++i)
        {
            auto fi = positions.find({i, j});
            if (fi == end(positions))
                outF << ". ";
            else
                outF << "# ";
        }
        outF << endl;
    }
    outF.close();
}

void Tick(DT & data)
{
    for (auto & d : data)
    {
        d.location.x += d.velocity.x;
        d.location.y += d.velocity.y;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ifstream f;
    f.open("inputR.txt");
    string s;
    
    DT stars;
    
    while (getline(f, s))
    {
        vector<string> v =
        regexMatch(s, R"(position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>)");
        
        Star star;
        star.location.x = stoi(v[0]);
        star.location.y = stoi(v[1]);
        star.velocity.x = stoi(v[2]);
        star.velocity.y = stoi(v[3]);
        
        int i = 0; ++i;
        
        stars.push_back(star);
    }
    
    int step = 0;
    while (true)
    {
        Render(stars, step++);
        Tick(stars);
    }
    
    return 0;
}
