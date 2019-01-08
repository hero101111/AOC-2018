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

	bool PreviousNodesAreVisited(string & currentNodeWalk)
	{
		for (auto * prevStep : previousSteps)
		{
			if (currentNodeWalk.find(prevStep->name) == string::npos)
				return false;
		}
		return true;
	}

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

	while (steps.size() > 0)
	{
		vector<Step *> candidates;
		for (auto & step : steps)
		{
			if (step.second.previousSteps.size() == 0)
				candidates.push_back(&step.second);
		}
		sort(candidates.begin(), candidates.end(), [](auto & d1, auto & d2) { return *d1 < *d2;  });
		if (candidates.size() > 0)
		{
			auto * cand = candidates[0];
			nodeWalk += cand->name;
			cand->Unlink();
			steps.erase(cand->name);
		}
	}

	//std::reverse(begin(executionStr), end(executionStr));

	cout << nodeWalk.size() << " " << steps.size() << endl;
	cout << nodeWalk << endl;

	ofstream outF("out.dat");
	outF << nodeWalk;
	outF.close();

	return 0;
}
