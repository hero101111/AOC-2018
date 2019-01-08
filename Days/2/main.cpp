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
#include <unordered_set>
#include <unordered_map>
#include <assert.h>

using namespace std;

size_t countDiff(string s1, string s2)
{
	assert(s1.size() == s2.size());
	int t = 0; size_t index = string::npos;
	for (size_t i = 0; i < s1.size(); ++i)
		if (s1[i] != s2[i])
		{
			t++;
			index = i;
		}
	return t == 1 ? index : string::npos;
}

int main(int argc, const char * argv[]) {
	ifstream iFile(R"(c:\aoc\day2_input.txt)");
	string crtLine;

	int count2 = 0, count3 = 0;
	vector<string> values;

	while (getline(iFile, crtLine))
	{
		values.push_back(crtLine);
		bool countsAs2 = false;
		bool countsAs3 = false;

		unordered_map<char, int> m;
		for (auto c : crtLine)
			m[c]++;

		for (auto kv : m)
		{
			if (kv.second == 2)
				countsAs2 = true;

			if (kv.second == 3)
				countsAs3 = true;
		}

		if (countsAs2) count2++;
		if (countsAs3) count3++;
	}

	bool foundAnswer = false;
	for (auto & s : values)
	{
		for (auto & s2 : values)
		{
			if (s == s2)
				continue;

			size_t c = countDiff(s, s2);
			if (c != string::npos)
			{
				s.erase(c, 1);
				cout << "Common letters: " << s << endl;
				foundAnswer = true;
				break;
			}
		}
		if (foundAnswer)
			break;
	}

	cout << "Checksum: " << (count2 * count3) << endl;
}
