// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

using namespace std;

struct Point
{
    int x{ 0 }, y{ 0 };
    
    Point() {}
    
    Point(int x, int y) { this->x = x; this->y = y; }
    
    Point(const Point & p) { x = p.x; y = p.y; }
    
    Point & operator =(const Point & p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
    
    bool operator ==(const Point & other) const
    {
        return x == other.x && y == other.y;
    }
    
    bool operator !=(const Point & other) const
    {
        return !operator==(other);
    }
    
    bool operator < (const Point & other) const
    {
        if (y < other.y) return true;
        if (y == other.y && x < other.x)
            return true;
        return false;
    }
    
    int ManhattanDist(const Point & p) const
    {
        return abs(x - p.x) + abs(y - p.y);
    }
    
    
    bool IsInGrid(int width, int height) const
    {
        return x >= 0 && y >= 0 && x < width && y < height;
    }
};

enum class UnitType
{
    Gremlin = 0,
    Elf,
    Wall,
    Free
};

using WT = set<Point>;

class BattleData;

struct Unit
{
    Point location;
    int hitPoints{ 200 };
    int attackPower{ 3 };
    UnitType type{ UnitType::Elf };
    
    BattleData * battle{ nullptr };
    
    bool operator ==(const Unit & other) const
    {
        return location == other.location && type == other.type;
    }
    
    bool operator < (const Unit & other) const
    {
        return location < other.location;
    }
    
    bool IsDead() const
    {
        return hitPoints <= 0;
    }
    
    bool IsEnemy(const Unit & u) const
    {
        assert(type == UnitType::Gremlin || type == UnitType::Elf);
        assert(u.type == UnitType::Gremlin || u.type == UnitType::Elf);
        return type != u.type;
    }
    
    void Attack(Unit & other)
    {
        other.hitPoints -= attackPower;
    }
};

using DT = vector<Unit>;

struct BattleData
{
    DT * units{ nullptr };
    WT * walls{ nullptr };
    
    int width{ 0 };
    int height{ 0 };
    int tickCount { 0 };
    bool failed {false};
    
    UnitType TypeAt(Point p, Unit ** outUnit = nullptr)
    {
        assert(p.x >= 0 && p.y >= 0);
        assert(p.x < width && p.y < height);
        
        if (walls->find(p) != end(*walls))
            return UnitType::Wall;
        auto ui = find_if(begin(*units), end(*units), [&](auto & u) { return u.location == p; });
        if (ui != end(*units) && !ui->IsDead())
        {
            if (outUnit != nullptr)
                *outUnit = &*ui;
            return ui->type;
        }
        
        return UnitType::Free;
    }
    
    Unit * UnitAt(Point p)
    {
        Unit * myUnit = nullptr;
        auto typeAt = TypeAt(p, &myUnit);
        assert(typeAt == UnitType::Gremlin || typeAt == UnitType::Elf);
        if (typeAt == UnitType::Gremlin || typeAt == UnitType::Elf)
            assert(myUnit != nullptr);
        return myUnit;
    }
    
    vector<Point> GetNeighbours(const Point & p)
    {
        vector<Point> ret;
        for (int oy = -1; oy <= 1; ++oy)
        {
            for (int ox = -1; ox <= 1; ++ox)
            {
                if (abs(ox + oy) % 2 == 0)
                    continue;
                
                Point neighP = p;
                
                neighP.x += ox;
                neighP.y += oy;
                
                if (!neighP.IsInGrid(width, height))
                    continue;
                
                auto typeAt = TypeAt(neighP);
                if (typeAt == UnitType::Free)
                {
                    ret.push_back(neighP);
                }
            }
        }
        return ret;
    }
    
    vector<Point> ShortPathTo(const Point & from, const Point & to)
    {
        map<Point, int> dist;
        map<Point, Point> prev;
        
        vector<Point> Q;
        vector<Point> ret;
        for (int x = 0; x < width; ++x)
            for (int y = 0; y < height; ++y)
            {
                dist[{x, y}] = numeric_limits<int>::max() - 1;
                prev[{x, y}] = {-1, -1};
                Q.push_back({x, y});
            }
        
        dist[from] = 0;
        while (!Q.empty())
        {
            auto minU = min_element(begin(Q), end(Q), [&](auto & q1, auto & q2)
                                    {
                                        if (dist[q1] < dist[q2])
                                            return true;
                                        if (dist[q1] == dist[q2])
                                            return q1 < q2;
                                        return false;
                                    });
            Point u = *minU;
            Q.erase(minU);
            
            if (u == to)
            {
                while ((u.x >= 0 && prev[u].x >= 0) || u == from)
                {
                    ret.insert(begin(ret), u);
                    u = prev[u];
                }
                return ret;
            }
            
            auto neigh = GetNeighbours(u);
            
            for (auto v : neigh)
            {
                int alt = dist[u] + 1;
                if (alt < dist[v])
                {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
        
        return ret;
    }
    
    bool TryToAttack(Unit & u)
    {
        Unit * enemy = nullptr;
        
        for (auto & e : *units)
        {
            if (e.IsDead())
                continue;
            if (&e == &u)
                continue;
            if (!u.IsEnemy(e))
                continue;
            if (u.location.ManhattanDist(e.location) == 1)
            {
                if (enemy == nullptr)
                {
                    enemy = &e;
                }
                else
                {
                    if (e.hitPoints < enemy->hitPoints)
                    {
                        enemy = &e;
                    }
                    else if (e.hitPoints == enemy->hitPoints)
                    {
                        if (e.location < enemy->location)
                            enemy = &e;
                    }
                }
            }
        }
        
        if (enemy)
        {
            u.Attack(*enemy);
            if (enemy->type == UnitType::Elf && enemy->IsDead())
                failed = true;
            return true;
        }
        return false;
    }
    
    bool Tick()
    {
        if (failed)
            return false;
        bool tickIsValid = false;
        
        sort(begin(*units), end(*units));
        
        for (auto & u : *units)
        {
            if (u.IsDead())
                continue;
            
            set<Point> potentialTargetsToMoveTo;
            
            bool roundEnded = false;
            
            if (TryToAttack(u))
            {
                // attack
                tickIsValid = true;
                continue;
            }
            
            for (auto & p : *units)
            {
                if (p.IsDead())
                    continue;
                
                if (&u == &p)
                    continue;
                
                if (!u.IsEnemy(p))
                    continue;
                
                vector<Point> uNeighbours = GetNeighbours(p.location);
                copy(begin(uNeighbours), end(uNeighbours), inserter(potentialTargetsToMoveTo, begin(potentialTargetsToMoveTo)));
            }
            
            if (potentialTargetsToMoveTo.empty())
                continue;
            
            if (roundEnded)
                continue;
            
            map<Point, int> distanceToTargets;
            map<Point, vector<Point>> targetPaths;
            
            vector<Point> actualTargets;
            
            for (auto & target : potentialTargetsToMoveTo)
            {
                vector<Point> path = ShortPathTo(u.location, target);
                if (path.size() > 0)
                {
                    distanceToTargets[target] = (int)path.size();
                    targetPaths[target] = path;
                    actualTargets.push_back(target);
                }
            }
            if (actualTargets.empty())
                continue;
            
            sort(begin(actualTargets), end(actualTargets), [&](auto & t1, auto & t2)
                 {
                     if (distanceToTargets[t1] < distanceToTargets[t2])
                         return true;
                     if (distanceToTargets[t1] == distanceToTargets[t2])
                         return t1 < t2;
                     return false;
                 });
            
            Point target = actualTargets.front();
            vector<Point> & pathToTarget = targetPaths[target];
            u.location = pathToTarget[1];
            tickIsValid = true;
            
            if (TryToAttack(u))
            {
                // attack
                tickIsValid = true;
            }
        }
        //tickIsValid ? tickCount++ : tickCount--;
        tickCount++;
        
        /* auto eraseBound = remove_if(begin(*units), end(*units), [](auto & u)
         {
         return u.IsDead();
         });
         if (eraseBound != end(*units))
         (*units).erase(eraseBound);*/
        
        return tickIsValid;
    }
    
    void Print()
    {
        cout << endl;
        cout << "Round: " << tickCount << endl;
        cout << "Remaining hit points: " << RemainingHitPoints() << endl;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                auto t = TypeAt({x, y});
                cout << ((t == UnitType::Free) ? '.' : (t == UnitType::Elf ? 'E' : (t == UnitType::Wall ? '#' : 'G')));
            }
            cout << endl;
        }
    }
    
    int GetPoints()
    {
        return tickCount * RemainingHitPoints();
    }
    
    int RemainingHitPoints()
    {
        int t = 0;
        for (auto & u : *units)\
        {
            if (u.IsDead())
                continue;
            t += u.hitPoints;
        }
        return t;
    }
};


/*DT GetDigits(int n)
 {
 DT  ret;
 if (n == 0) ret.push_back(0);
 while (n > 0)
 {
 ret.insert(begin(ret), n % 10);
 n /= 10;
 }
 //reverse(begin(ret), end(ret));
 return ret;
 }*/

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
    
    int elfPower = 23;
    
    for (; ; elfPower++)
    {
        cout << "CURRENT POWER: " << elfPower << endl;
        ifstream f;
        
        bool demo = false;
        
        f.open(demo ? R"(day15d.txt)" : R"(day15r.txt)");
        string s;
        
        DT units;
        WT walls;
        
        BattleData battle;
        battle.walls = &walls;
        battle.units = &units;
        
        
        int y = 0;
        for (; getline(f, s); ++y)
        {
            //vector<string> tokens = regexMatch(s, R"()");
            
            if (battle.width == 0)
                battle.width = (int)s.size();
            
            for (int x = 0; x < s.size(); ++x)
            {
                switch (s[x])
                {
                    case '#':
                        walls.insert({ x, y });
                        break;
                    case 'G':
                    case 'E':
                    {
                        Unit unit;
                        unit.type = s[x] == 'E' ? UnitType::Elf : UnitType::Gremlin;
                        unit.location = { x, y };
                        unit.battle = &battle;
                        if (unit.type == UnitType::Elf) unit.attackPower = elfPower;
                        
                        units.push_back(unit);
                        break;
                    }
                    case '.':
                        break;
                }
            }
        }
        battle.height = y;
        
        //auto u2 = battle.UnitAt({ 8, 8 });
        
        //vector<Point> track {};
        //auto a = battle.TypeAt({4, 6});
        // battle.UpdateMap();
        battle.Print();
        // auto t = battle.ShortPathTo({ 4, 4 }, { 7, 5 });
        
        for (; battle.Tick(); battle.Print());
        // battle.UpdateMap();
        battle.Print();
        cout << battle.GetPoints() << endl;
        
        if (!battle.failed)
            break;
    }
    cout << endl << "ELF POWER: " << elfPower << endl;
    
    
    return 0;
}
