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

using namespace std;

int main(int argc, const char * argv[]) {

	bool foundFirst = false;
	bool foundSecond = false;

	int crtFreq = 0;
	unordered_set<int> frequenciesReached;
	frequenciesReached.insert(crtFreq);

	while (true)
	{
		ifstream iFile(R"(day1_input.txt)");
		string crtVal;

		while (getline(iFile, crtVal))
		{
			crtFreq += stoi(crtVal);
			if (frequenciesReached.find(crtFreq) != end(frequenciesReached) && !foundSecond)
			{
				cout << "First frequency reached twice is: " << crtFreq << endl;
				foundSecond = true;
			}

			frequenciesReached.insert(crtFreq);
		}

		if (!foundFirst)
		{
			cout << "Freq Count: " << crtFreq << endl;
			foundFirst = true;
		}

		if (foundFirst && foundSecond)
			break;
	}	
}
