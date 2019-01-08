// ConsoleApplication6.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include <queue>
#include <deque>
#include <stack>
#include <numeric>
#include <list>
#include <tgmath.h>
#include <assert.h>
#include <deque>
#include <bitset>
#include <unordered_map>

using namespace std;
int main()
{
	/*long long r0 = 0;
	long long r1 = 1;
	long long r3 = 9107763;
	long long r4 = 1;
	long long r5 = 1;*/
	long long r0 = 0;
	long long r3 = 9107763;
	long long r4 = 1;

	long long maxr3 = numeric_limits<int>::min();
	//long long lastDuration = numeric_limits<long long>::min();

	ofstream fout("c:\\aoc\\dd.txt");

	set <long long > found;
	long long prev;

	goto Line28;


Line6: 	
	r4 = r3 | 65536;
	r3 = 7041048; 
Line8:
	r3 += (r4 & 255);
	r3 &= 16777215; 
	r3 *= 65899; 
	r3 &= 16777215; 

	if (256 > r4)
		goto Line28;
	
	r4 = r4 >> 8;
	goto Line8;

Line28:
	
	if (found.find(r3) != end(found))
	{
		cout << "A: " << prev << endl;
		return 0;
	}

	found.insert(r3);
	prev = r3;
	if (r3 > maxr3)
	{
		maxr3 = r3;
		cout << maxr3 << endl;
	//	std::string binary = std::bitset<32>(r3).to_string(); //to binary
	//	std::cout << binary << "  " << r3 << "\n";

		//std::cout << std::bitset<32>(r3 | 16777215).to_string() << "  " << (r3 | 16777215) << "\n";
		//std::cout << std::bitset<32>(r3 & 16777215).to_string() << "  " << (r3 & 16777215) << "\n";
		//std::cout << std::bitset<32>(r3 ^ 16777215).to_string() << "  " << (r3 ^ 16777215) << "\n";

		cout << endl;
	}
	if (r3 == r0)
		return 0;
	else
		goto Line6;
}