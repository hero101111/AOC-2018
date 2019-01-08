//
//  main.cpp
//  aoc1
//
//  Created by Gabriel on 01/12/2018.
//  Copyright © 2018 Gabriel. All rights reserved.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <algorithm>
#include <numeric>

using namespace std;

struct Date
{
	int year, month, day, minute, hour;

	Date() { }

	Date(const Date & d)
		: year(d.year), month(d.month), day(d.day), minute(d.minute), hour(d.hour)
	{
	}

	bool operator ==(const Date & other) const
	{
		return year == other.year && month == other.month && day == other.day && minute == other.minute && hour == other.hour;
	}

	bool operator < (const Date & other) const
	{
		if (year < other.year)
			return true;
		if (year == other.year && month < other.month)
			return true;
		if (year == other.year && month == other.month && day < other.day)
			return true;
		if (year == other.year && month == other.month && day == other.day && hour < other.hour)
			return true;
		if (year == other.year && month == other.month && day == other.day && hour == other.hour && minute < other.minute)
			return true;

		return false;
	}
};

struct Event
{
	enum class Type
	{
		BeginShift,
		FallAsleep,
		WakeUp
	};

	string eventDetails;
	Date timeStamp;
	Type type;

	bool operator < (const Event & other) const
	{
		return timeStamp < other.timeStamp;
	}
};

struct Guard
{
	vector<Event> events;
	map<int, int> sleepMap;

	int id;

	void CalculateSleepMap()
	{
		int currentMinute = 0;
		bool isSleeping = false;

		for (auto & ev : events)
		{
			if (ev.type == Event::Type::BeginShift)
			{
				if (isSleeping)
				{
					for (int i = currentMinute; i < 60; ++i)
						sleepMap[i]++;
				}
				currentMinute = 0;
				isSleeping = false;
				continue;
			}

			if (ev.type == Event::Type::FallAsleep)
			{
				currentMinute = ev.timeStamp.minute;
				isSleeping = true;
			}

			if (ev.type == Event::Type::WakeUp)
			{
				for (int i = currentMinute; i < ev.timeStamp.minute; ++i)
					sleepMap[i]++;
				isSleeping = false;
				currentMinute = ev.timeStamp.minute;
			}
		}

		if (isSleeping)
		{
			for (int i = currentMinute; i < 60; ++i)
				sleepMap[i]++;
		}
	}

	int GetTotalSleep()
	{
		return accumulate(begin(sleepMap), end(sleepMap), 0, [](int t, auto & m) { return t + m.second; });
	}

	bool operator < (const Guard & g) const
	{
		return id < g.id;
	}

	bool operator == (const Guard & g) const
	{
		return id == g.id;
	}
};



int main(int argc, const char * argv[]) {
	// insert code here...
	ifstream f;
	f.open("c:\\aoc\\day3\\debug\\day4.txt");
	string s;

	int lastGuardId = 0;
	map<int, Guard> guards;
	set<Event> orderedEvents;

	while (getline(f, s))
	{
		std::string result;
		std::regex re(R"(\[(\d+)\-(\d+)\-(\d+)\s(\d+):(\d+)\] (.+))");
		std::smatch match;
		std::regex_search(s, match, re);

		Date eventStamp;
		eventStamp.year = stoi(match.str(1));
		eventStamp.month = stoi(match.str(2));
		eventStamp.day = stoi(match.str(3));
		eventStamp.hour = stoi(match.str(4));
		eventStamp.minute = stoi(match.str(5));

		string eventDetail = match.str(6);

		Event event;
		event.eventDetails = eventDetail;
		event.timeStamp = eventStamp;
		if (eventDetail == "wakes up")
			event.type = Event::Type::WakeUp;
		if (eventDetail == "falls asleep")
			event.type = Event::Type::FallAsleep;
		if (eventDetail.find("#") != string::npos)
			event.type = Event::Type::BeginShift;
		orderedEvents.insert(event);
	}

	for (auto & event : orderedEvents)
	{
		if (event.type == Event::Type::BeginShift)
		{
			// new guard
			std::regex re2(R"(#(\d+))");
			std::smatch match2;
			std::regex_search(event.eventDetails, match2, re2);

			int guardId = stoi(match2.str(1));
			lastGuardId = guardId;

			if (guards.find(lastGuardId) == end(guards))
			{
				Guard guard;
				guard.id = guardId;
				guard.events.push_back(event);
				guards[lastGuardId] = guard;
			}
		}
		else
		{
			Guard & g = guards[lastGuardId];
			g.events.push_back(event);
		}
	}

	for (auto & g : guards)
	{
		g.second.CalculateSleepMap();
		//cout << g.second.GetTotalSleep() << "   " << g.first << endl;
	}

	/*vector<Guard> sortedGuards;
	transform(begin(guards), end(guards), back_inserter(sortedGuards), [](auto & mv) { return mv.second; });
	sort(begin(sortedGuards), end(sortedGuards), [](auto & g1, auto g2) { return g2.GetTotalSleep() < g1.GetTotalSleep(); });*/

	int maxId = 0, maxMinute = -1, maxValue = -1;
	for (auto & g : guards)
	{
		for (auto & mv : g.second.sleepMap)
		{
			if (mv.second > maxValue)
			{
				maxId = g.first;
				maxMinute = mv.first;
				maxValue = mv.second;
			}
		}
	}

	cout << "ID: " << maxId << " MIN: " << maxMinute << endl;

	return 0;
}
