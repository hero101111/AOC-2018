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

int react(string s)
{
    while (true)
    {
        bool found = false;
        for (int i = 0; i < s.size() - 1; ++i)
        {
            if (abs((int)s[i] - (int)s[i+1]) == 32)
            {
                s.erase(s.begin() + i, s.begin() + i + 2);
                found  = true;
                continue;
            }
        }
        if (!found)
            break;
      }
    return s.size();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    ifstream f;
    f.open("day5.txt");
    string s;
    
    getline(f, s);
   
    
    cout << "F"<< endl;
    
    size_t minL = numeric_limits<size_t>::max();
    
    for (int i = 65; i <= 90; ++i)
    {
        string s2 = s;
        while (true)
        {
            bool found = false;
            auto it1 = s2.find((char)i);
            if (it1 != string::npos)
            {
                s2.erase(s2.begin() + it1, s2.begin() + it1 + 1);
                found = true;
            }
            auto it2 = s2.find((char)(i+32));
            if (it2 != string::npos)
            {
                s2.erase(s2.begin() + it2, s2.begin() + it2 + 1);
                found = true;
            }
            if (!found)
                break;
        }
        
        int newSize = react(s2);
        if (newSize < minL)
        {
            minL = newSize;
        }
    }
    
    cout << minL << endl;
   
   // cout << s << endl;
   // cout << s.size() << endl;
    
    return 0;
}
