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
    int x, y, z;
    
    bool operator ==(const Point & other) const
    {
        return x == other.x && y == other.y && z == other.z;
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
        return abs(x - p.x) + abs(y - p.y) + abs(z - p.z);
    }
};

string lowerstring(string s)
{
    string newS = s;
    transform(begin(newS), end(newS), begin(newS), ::tolower);
    return newS;
}

struct Group
{
    int id {0};
    bool isInfection {false};
    
    int unitCount;
    
    int hitPoints;
    
    int attackDamage;
    string attackType;
    int initiative;
    
    string weakTo;
    string immuneTo;
    
    bool targeted { false };
    int currentTargetId { 0 };
    
    int getEffectivePower() const
    {
        return (int)unitCount * attackDamage;
    }
    
    bool isImmuneTo(string s)
    {
        return immuneTo.find(s) != string::npos;
    }
    
    bool isWeakTo(string s)
    {
        return weakTo.find(s) != string::npos;
    }
    
    int IncurLosses(int damage)
    {
        int loss = damage / hitPoints;
        unitCount -= loss;
        return unitCount;
    }
    
    bool IsDead() const
    {
        return unitCount <= 0;
    }
    
    int GetPotentialDamageToEnemy(Group & enemy)
    {
        if (enemy.isImmuneTo(attackType))
            return 0;
        
        int power = getEffectivePower();
        if (enemy.isWeakTo(attackType))
            return 2 * power;
        
        return power;
    }
};

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
    ofstream iOutFile("out24.txt");
    
    int count = 0;
    
    iOutFile << "COUNT: " << count << endl;
    iOutFile.close();
}

using DT = vector<Group>;

DT groups;
//DT infection;

Group * HeTarget(Group & group)
{
    Group * t = nullptr;
    for (auto & e : groups)
    {
        if (e.IsDead()) continue;
        if (e.targeted) continue;
        if (e.isInfection == group.isInfection) continue;
        
        int damageTo = group.GetPotentialDamageToEnemy(e);
        if (damageTo == 0)
            continue;
        
        if (!t)
        {
            t = &e;
            continue;
        }
        
        int damageToT = group.GetPotentialDamageToEnemy(*t);
        if (damageTo > damageToT)
        {
            t = &e;
            continue;
        }
        
        if (damageTo == damageToT)
        {
            int tEp = t->getEffectivePower();
            int ep = e.getEffectivePower();
            if (ep > tEp)
            {
                t = &e;
                continue;
            }
            
            if (ep == tEp && e.initiative > t->initiative)
            {
                t = &e;
                continue;
            }
        }
    }
    
    if (t)
        t->targeted = true;
    return t;
}

void Tick()
{
    for (auto & e : groups)
    {
        e.targeted = false;
        e.currentTargetId = -1;
    }
    
    sort(begin(groups), end(groups), [](Group & g1, Group & g2)
    {
        auto g1ep = g1.getEffectivePower();
        auto g2ep = g2.getEffectivePower();
        if (g2ep < g1ep)
            return true;
        if (g2ep == g1ep)
            return g2.initiative < g1.initiative;
        return false;
    });
    
    for (auto & g : groups)
    {
        if (g.IsDead()) continue;
        Group * target = HeTarget(g);
        if (target)
        {
            g.currentTargetId = target->id;
        }
    }
    
    sort(begin(groups), end(groups), [](Group & g1, Group & g2)
         {
             return g2.initiative < g1.initiative;
         });
    
    for (auto & attackG : groups)
    {
        if (attackG.IsDead()) continue;
        if (attackG.currentTargetId < 0)
            continue;
        
        auto foundTarget = find_if(begin(groups), end(groups), [&](Group & g){return !g.IsDead() && g.id == attackG.currentTargetId;});
        assert(foundTarget != end(groups));
        Group & target = *foundTarget;
        int damage = attackG.GetPotentialDamageToEnemy(target);
        target.IncurLosses(damage);
    }
    
    groups.erase(remove_if(begin(groups), end(groups), [](Group & g){ return g.IsDead(); }), end(groups));
}

static int boost = 0;

void ReadData(string inputFile, bool infection)
{
    string mainRegex = R"((\d+) units each with (\d+) hit points ?\(?(.+)?\)? with an attack that does (\d+) (.+) damage at initiative (\d+))";
    
    ifstream immuneFile(inputFile);
    string readLine;
    static int currentid = 0;
    while (getline(immuneFile, readLine))
    {
        readLine = lowerstring(readLine);
        auto tokens = regexMatch(readLine, mainRegex);
        Group group;
        group.id = currentid++;
        group.isInfection = infection;
        group.unitCount = stoi(tokens[0]);
        group.hitPoints = stoi(tokens[1]);
        // tokens[2] is list of weak and immune
        vector<string> innerStrings;
        size_t semicolon = tokens[2].find(';');
        if (semicolon != string::npos)
        {
            innerStrings.push_back(tokens[2].substr(0, semicolon));
            innerStrings.push_back(tokens[2].substr(semicolon + 1));
        }
        else
        {
            innerStrings.push_back(tokens[2]);
        }
        for (auto & s : innerStrings)
        {
            if (s.find("weak") != string::npos)
                group.weakTo = s;
            if (s.find("immune") != string::npos)
                group.immuneTo = s;
        }
        
        
        group.attackDamage = stoi(tokens[3]);
        if (!infection)
            group.attackDamage += boost;
        
        group.attackType = tokens[4];
        group.initiative = stoi(tokens[5]);
        
        groups.push_back(group);
    }
}

int UnitsLeft()
{
    int s = 0;
    for (Group & g : groups)
    {
        if (!g.IsDead())
            s += g.unitCount;
    }
    return s;
}

size_t getCount(bool infection)
{
    return count_if(begin(groups), end(groups), [&](Group & g){ return !g.IsDead() && g.isInfection == infection; });
}

int main(int argc, const char * argv[]) {
    
    // insert code here...
    string line;
    bool demo = false;
    
    boost = 51;
    while (true)
    {
        groups.clear();
        if (demo)
        {
            ReadData("day24d_immune.txt", false);
            ReadData("day24d_infection.txt", true);
        }
        else
        {
            ReadData("day24r_immune.txt", false);
            ReadData("day24r_infection.txt", true);
        }
        
        size_t immCount = getCount(false), infCount = getCount(true);
        while (immCount * infCount > 0)
        {
            Tick();
            
            immCount = getCount(false);
            infCount = getCount(true);
            cout << boost << "  imm count: " << immCount << "; infCount: " << infCount << endl;
        }
        
        cout << "units left: " << UnitsLeft() << endl;
        
        
        if (immCount > 0)
            break;
        
        cout << "new boost: " << ++boost << endl;
    }
    
    return 0;
}
