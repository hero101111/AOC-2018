//
//  main.cpp
//  aoc1
//
//  Created by Gabriel on 01/12/2018.
//  Copyright © 2018 Gabriel. All rights reserved.
//

#include "pch.h"
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

using namespace std;


struct Step
{
	string name;
	vector<string> depsStr;

	vector<Step *> nextSteps;
	vector<Step *> previousSteps;

	bool operator == (const Step & other) const
	{
		return name == other.name;
	}

	bool operator < (const Step & other) const
	{
		return name < other.name;
	}

	void Unlink()
	{
		for (auto & nextStep : nextSteps)
		{
			nextStep->previousSteps.erase(find(nextStep->previousSteps.begin(), nextStep->previousSteps.end(), this));
		}
		for (auto & prevStep : previousSteps)
		{
			prevStep->nextSteps.erase(find(prevStep->nextSteps.begin(), prevStep->nextSteps.end(), this));
		}
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

struct Worker
{
	int id{ 0 }, workload{ 0 };
	Step * step{ nullptr };

	Worker(int id) { this->id = id; };

	bool operator == (const Worker & other) const
	{
		return id == other.id && workload == other.workload && step == other.step;
	}
};

int main(int argc, const char * argv[]) {
	// insert code here...
	ifstream f;
	f.open(R"(c:\aoc\day7.txt)");
	string s;

	map<string, Step> steps;
	while (getline(f, s))
	{
		vector<string> stepS = regexMatch(s, R"(Step (\S) must be finished before step (\S) can begin.)");
		Step & s = steps[stepS[0]];
		s.name = stepS[0];
		s.depsStr.push_back(stepS[1]);

		Step & s2 = steps[stepS[1]];
		s2.name = stepS[1];
	}

	// validate input
	for (auto & s : steps)
	{
		for (auto & depStr : s.second.depsStr)
		{
			Step & step1 = s.second;
			Step & step2 = steps[depStr];

			step1.nextSteps.push_back(&step2);
			step2.previousSteps.push_back(&step1);
		}
	}
	for (auto & s : steps)
	{
		sort(begin(s.second.nextSteps), end(s.second.nextSteps),
			[](auto & d1, auto & d2) { return *d1 < *d2;  });
	}

	//-----------------------------------------------------------------------------------------------

	string nodeWalk;

	vector<Worker> workers = { 1, 2, 3, 4, 5 };

	int currentSecond = 0;

	while (steps.size() > 0)
	{
		vector<Worker *> freeWorkers;
		for (auto & worker : workers)
		{
			if (worker.workload == 0)
				freeWorkers.push_back(&worker);
		}

		vector<Step *> candidates;
		for (auto & step : steps)
		{
			if (step.second.previousSteps.size() == 0)
			{
				bool taken = false;
				for (auto & worker : workers)
				{
					if (worker.workload > 0 && worker.step->name == step.second.name)
					{
						taken = true;
						break;
					}
				}
				if (!taken)
					candidates.push_back(&step.second);
			}
		}
		sort(candidates.begin(), candidates.end(), [](auto & d1, auto & d2) { return *d1 < *d2;  });

		if (candidates.size() > 0)
		{
			for (auto * candidate : candidates)
			{
				if (freeWorkers.empty())
					break;

				freeWorkers.back()->workload = candidate->name[0] - 64 + 60;
				freeWorkers.back()->step = candidate;
				freeWorkers.pop_back();
			}
		}

		currentSecond++;
		for (auto & worker : workers)
		{
			if (worker.workload > 0)
			{
				worker.workload--;
				if (worker.workload == 0)
				{
					nodeWalk += worker.step->name;
					worker.step->Unlink();
					steps.erase(worker.step->name);
					worker.step = nullptr;
				}
			}
		}
	}

	cout << currentSecond << endl;

	//std::reverse(begin(executionStr), end(executionStr));

	cout << nodeWalk.size() << " " << steps.size() << endl;
	cout << nodeWalk << endl;

	ofstream outF("out.dat");
	outF << nodeWalk;
	outF.close();

	return 0;
}
