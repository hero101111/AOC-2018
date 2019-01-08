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

using DT = list<int>;

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

// + is clockwise
DT::iterator getMarbleInsertionPoint(DT & marbles, DT::iterator start, size_t offset, bool clockwise)
{
    auto startPoint = start;
    size_t inc = 0;
    for (; inc < offset; ++inc)
    {
        if (clockwise)
        {
            ++startPoint;
            if (startPoint == end(marbles))
                startPoint = begin(marbles);
        }
        else
        {
            if (startPoint == begin(marbles))
                startPoint = end(marbles);
            --startPoint;
        }
    }
    return startPoint;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ifstream f;
    f.open("input.txt");
    string s;
    
   /* while (getline(f, s))
    {
        vector<string> v = regexMatch(s, R"(Step (\S) must be finished before step (\S) can begin.)");
    }*/
    
    int n = 465; // players
    int endMarble = 7194000;
    
    int currentPlayer = 0;
    unsigned long long scores[500] = {0};
    
    
    int newMarble = 1;
    DT marbles = { 0 };
    auto currentMarble = marbles.begin();
    while (*currentMarble < endMarble)
    {
        if (newMarble % 23 != 0)
        {
            auto insertionPoint = getMarbleInsertionPoint(marbles, currentMarble, 1, true);
            insertionPoint++;
            currentMarble = marbles.insert(insertionPoint, newMarble);
        }
        else
        {
            auto marbleToRemove = getMarbleInsertionPoint(marbles, currentMarble, 7, false);
            
            scores[currentPlayer] += *marbleToRemove;
            scores[currentPlayer] += newMarble;
            
            currentMarble = marbles.erase(marbleToRemove);
        }
                   
        currentPlayer = (currentPlayer + 1) % n;
        newMarble++;
    }
    
    cout << *std::max_element(begin(scores), end(scores)) << endl;
    
    
    
    return 0;
}
