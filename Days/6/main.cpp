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

using namespace std;


int manDist(int x1, int x2, int y1, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

struct Point
{
    int x, y, id;
    
    bool operator == (const Point & other) const
    {
        return x== other.x && y == other.y;
    }
};

int getArea(int w, int h)
{
    return w * h;
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
    ifstream f;
    f.open("input.txt");
    string s;
    
    vector<Point> points;
    int maxX = -1, maxY = -1, crtId = 1;
    while (getline(f, s))
    {
        vector<string> strPoint = regexMatch(s, R"((\d+),\s(\d+))");
        Point p;
        p.y = stoi(strPoint[0]);
        p.x = stoi(strPoint[1]);
        p.id = crtId++;
        points.push_back(p);
        
        if (p.x > maxX)
            maxX = p.x;
        if (p.y > maxY)
            maxY = p.y;
    }
    
    if (maxX > maxY)
        maxY = maxX;
    if (maxY > maxX)
        maxX = maxY;
    cout << "MAX X: " << maxX << endl;
    cout << "MAX Y: " << maxY << endl;
    
    
    cout << "PT : " << points.size() << endl;
   
    int A1 = 0;
    string A1S = "";
    // ---------------------------------------------
    // xxx part I
    
    int v[410][410] = {0};
    for (int i = 0; i < maxX; ++i)
    {
        for (int j = 0; j < maxY; ++j)
        {
            int minManhattanDist = numeric_limits<int>::max();
            int minManhattanId = 0;
            
            bool isPoint = false;
            
            vector<int> distances;
            for (auto & p : points)
            {
                if (p.x == i && p.y == j)
                {
                    isPoint = true;
                    v[i][j] = p.id;
                    continue;
                }
                
                int newDist = manDist(i, p.x, j, p.y);
                distances.push_back(newDist);
                
                if (newDist < minManhattanDist)
                {
                    minManhattanDist = newDist;
                    minManhattanId = p.id;
                }
            }
            
            if (v[i][j] == 0)
            {
                int countD = count(begin(distances), end(distances), minManhattanDist);
                
                if (countD == 1)
                    v[i][j] = minManhattanId;
                else
                    v[i][j] = -1;
            }
        }
    }
    
    // ---------------------------------------------
    //cout << "A1 : " << A1 << endl;
    //cout << "A1S: " << A1S << endl;
    cout << "-----------------------------" << endl;
    int A2 = 0;
    string A2S = "";
    
    int maxArea = numeric_limits<int>::min();
    
    /*
    for (int i = 1; i <= maxX; ++i)
    {
        for (int j = 1; j <= maxY; ++j)
        {
            int currentCellId = v[i][j];
            bool consistentValues = true;
            
            for (int m = i + 1; m <= maxX; ++m)
            {
                for (int n = j + 1; n <= maxY; ++n)
                {
                    if (v[m][n]!=currentCellId)
                    {
                        if (n - j > 0 && m - i > 0)
                        {
                            int area = getArea(m - i, n - j - 1);
                            if (area > maxArea)
                                maxArea = area;
                        }
                        consistentValues = false;
                        break;
                    }
                }
                if (!consistentValues)
                {
                    if (m - i > 0)
                    {
                        int area = getArea(m - i, j);
                        if (area > maxArea)
                            maxArea = area;
                    }
                    break;
                }
            }
        }
    }*/
    
    ofstream fileOut("data.out");
    for (int i = 0; i < maxX; ++i)
    {
        for (int j = 0; j < maxY; ++j)
        {
            fileOut << v[i][j] << " ";
        }
        fileOut << endl;
    }
    fileOut.close();
    
    int maxId = 0;
    for (auto & p : points)
    {
        int area = 0;
        bool isInfinite = false;
        for (int i = 0; i < maxX; ++i)
        {
            for (int j = 0; j < maxY; ++j)
            {
                if (v[i][j]==p.id)
                {
                    if (i == 0 || j == 0 || i == maxX - 1 || j == maxY - 1)
                        isInfinite = true;
                    area++;
                }
            }
        }
        if (area > maxArea && !isInfinite)
        {
            maxArea = area;
            maxId = p.id;
        }
    }
    
    cout << "MAX AREA: " << maxArea << endl;
    cout << "MAX ID: " << maxId << endl;
    
    
    // ---------------------------------------------
    // xxx part II
    
    
    
    // ---------------------------------------------
    //cout << "A2 : " << A2 << endl;
    //cout << "A2S: " << A2S << endl;
    
    return 0;
}
