// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include <stack>
#include <numeric>
#include <list>
#include <tgmath.h>
#include <assert.h>

using namespace std;

using DT = vector<char>;
using DTP = pair<DT::const_iterator, DT::const_iterator>;

DT GetDigits(int n)
{
	DT  ret;
	while (n > 0)
	{
		ret.insert(begin(ret), n % 10);
		n /= 10;
	}
	//reverse(begin(ret), end(ret));
	return ret;
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

DTP AdvanceElfs(DT & data, DTP elfs)
{
	DTP newElfs = elfs;
	int elf1_jump = 1 + *elfs.first;
	int elf2_jump = 1 + *elfs.second;
	while (elf1_jump-- > 0)
	{
		newElfs.first++;
		if (newElfs.first == data.end())
			newElfs.first = data.begin();
	}
	while (elf2_jump-- > 0)
	{
		newElfs.second++;
		if (newElfs.second == data.end())
			newElfs.second = data.begin();
	}
	return newElfs;
}

DTP NewRecipe(DT & data, DTP elfs)
{
	int elf_sum = *elfs.first + *elfs.second;

	DT newRecipes = GetDigits(elf_sum);
	if (elf_sum == 0)
		newRecipes.push_back(0);

	size_t elf1_offset = elfs.first - data.begin();
	size_t elf2_offset = elfs.second - data.begin();
	for (auto r : newRecipes)
		data.insert(end(data), r);

	DTP newElfs = { data.begin() + elf1_offset, data.begin() + elf2_offset };
	return AdvanceElfs(data, newElfs);
}


int main(int argc, const char * argv[]) {
	// insert code here...
	ifstream f;
	//f.open(R"(C:\aoc\day14r.txt)");
	f.open(R"(C:\aoc\day14d.txt)");
	string s;

	int n = 652601;
	DT nData = GetDigits(n);

	DT data = { 3, 7 };
	//int n = 2018;

	//data.reserve(n + 100);
	/*while (getline(f, s))
	{
	}*/

	DTP elfs{ data.begin(), data.begin() };
	elfs.second++;

	ofstream outF("c:\\aoc\\num.out");
	while (true)
	{
		elfs = NewRecipe(data, elfs);

		if (data.size() % 10'000'000 == 0)
		{
			cout << data.size() << endl;
		}
		//if (data.size() < 3'200'000'000u)
		//	continue;

		if (data.size() >= nData.size())
		{
			bool found = true;
			auto it1 = data.end();
			auto it2 = nData.end();
			for (int i = 1; i <= nData.size(); ++i)
			{
				if (*(--it1) != *(--it2))
					found = false;
			}
			if (found)
				break;
		}
	}
	cout << "S: " << data.size() - nData.size() << endl;
	cout << "data: " << data.size() << endl;
	for (size_t i = data.size() - 1; i > data.size() - 10; i--)
		cout << (int)data[i];

	cout << endl;
	/*for (int i = n; i < data.size(); ++i)
	{
		cout << data[i];
	}*/

	for (int i : data)
	{
		outF << i;
	}
	outF.close();

	return 0;
}
