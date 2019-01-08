// aoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
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
#include <deque>

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

enum class InstructionType
{
	addr = 10,
	addi = 6,
	mulr = 9,
	muli = 0,
	banr = 14,
	bani = 2,
	borr = 11,
	bori = 12,
	setr = 15,
	seti = 1,
	gtir = 7,
	gtri = 3,
	gtrr = 4,
	eqir = 8,
	eqri = 13,
	eqrr = 5
};

void ExecuteInstruction(vector<unsigned long long> & registers, int regA, int regB, int regC,
	InstructionType instr)
{
	if (instr == InstructionType::addr)
	{
		//registers[regA] = iP;
		registers[regC] = registers[regA] + registers[regB];
	}
	else if (instr == InstructionType::addi)
	{
		//registers[regA] = iP;
		registers[regC] = registers[regA] + regB;
	}
	else if (instr == InstructionType::mulr)
	{
		registers[regC] = registers[regA] * registers[regB];
	}
	else if (instr == InstructionType::muli)
	{
		registers[regC] = registers[regA] * regB;
	}
	else if (instr == InstructionType::banr)
	{
		registers[regC] = registers[regA] & registers[regB];
	}
	else if (instr == InstructionType::bani)
	{
		registers[regC] = registers[regA] & regB;
	}
	else if (instr == InstructionType::borr)
	{
		registers[regC] = registers[regA] | registers[regB];
	}
	else if (instr == InstructionType::bori)
	{
		registers[regC] = registers[regA] | regB;
	}
	else if (instr == InstructionType::setr)
	{
		registers[regC] = registers[regA];
		//iP = registers[regC];
	}
	else if (instr == InstructionType::seti)
	{
		registers[regC] = regA;
		//iP = registers[regC];
	}
	else if (instr == InstructionType::gtir)
	{
		registers[regC] = regA > registers[regB] ? 1 : 0;
	}
	else if (instr == InstructionType::gtri)
	{
		registers[regC] = registers[regA] > regB ? 1 : 0;
	}
	else if (instr == InstructionType::gtrr)
	{
		registers[regC] = registers[regA] > registers[regB] ? 1 : 0;
	}
	else if (instr == InstructionType::eqir)
	{
		registers[regC] = regA == registers[regB] ? 1 : 0;
	}
	else if (instr == InstructionType::eqri)
	{
		registers[regC] = registers[regA] == regB ? 1 : 0;
	}
	else if (instr == InstructionType::eqrr)
	{
		registers[regC] = registers[regA] == registers[regB] ? 1 : 0;
	}
	else
		assert(!"Unsupported");
}

InstructionType InstructionFromString(string instruction)
{
	if (instruction == "addr") return InstructionType::addr;
	else if (instruction == "addi") return InstructionType::addi;
	else if (instruction == "mulr") return InstructionType::mulr;
	else if (instruction == "muli") return InstructionType::muli;
	else if (instruction == "banr") return InstructionType::banr;
	else if (instruction == "bani") return InstructionType::bani;
	else if (instruction == "borr") return InstructionType::borr;
	else if (instruction == "bori") return InstructionType::bori;
	else if (instruction == "setr") return InstructionType::setr;
	else if (instruction == "seti") return InstructionType::seti;
	else if (instruction == "gtir") return InstructionType::gtir;
	else if (instruction == "gtri") return InstructionType::gtri;
	else if (instruction == "gtrr") return InstructionType::gtrr;
	else if (instruction == "eqir") return InstructionType::eqir;
	else if (instruction == "eqri") return InstructionType::eqri;
	else if (instruction == "eqrr") return InstructionType::eqrr;
}

struct Instruction
{
	InstructionType opcode;
	int registerA;
	int registerB;
	int registerC;
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

	int r0 = 0;
	int r1 = 1;
	//int r2 = 0;
	int r3 = 9107763;
	int r4 = 1;
	int r5 = 1;

	r4 = r3 | 65536;
	r3 = 7041048;
	r5 = r4 & 255;
	r3 = r3 + r5;
	r3 &= 16777215;
	r3 *= 65899;
	r3 &= 16777215;

	int i = 0;
	// insert code here...
	ifstream f;
	f.open("c:\\aoc\\day21_r.txt");

	string line;

	vector<Instruction> instructions;

	while (getline(f, line))
	{
		vector<string> instructionStr =
			regexMatch(line, R"((\S+) (\d+) (\d+) (\d+))");
		Instruction inst;

		inst.opcode = InstructionFromString(instructionStr[0]);
		inst.registerA = stoi(instructionStr[1]);
		inst.registerB = stoi(instructionStr[2]);
		inst.registerC = stoi(instructionStr[3]);

		instructions.push_back(inst);
		//ExecuteInstruction(registers, regA, regB, regC, (InstructionType)opcode);

	}


	int iPslot = 2;

	int iP = 0;

	vector<unsigned long long> registers = { 0, 0, 0, 0, 0, 0 };
	vector<unsigned long long> oldRegisters = { 0, 0, 0, 0, 0, 0 };
	int oldIp = 0;

	ofstream fout("c:\\aoc\\out21_0.txt");

	deque<int> history;

	unsigned long long max_v = numeric_limits<unsigned long long>::min();
	unsigned long long tick = 1;
	while (true)
	{
		oldIp = iP;
		Instruction & i = instructions[iP];

		registers[iPslot] = iP;

		oldRegisters[0] = registers[0];
		oldRegisters[1] = registers[1];
		oldRegisters[2] = registers[2];
		oldRegisters[3] = registers[3];
		oldRegisters[4] = registers[4];
		oldRegisters[5] = registers[5];


		//history.push_back(iP);

		ExecuteInstruction(registers, i.registerA, i.registerB, i.registerC, i.opcode);

	


		iP = registers[iPslot] + 1;

		/*fout << endl << endl<< "OiP: " << oldIp << " -> " << iP << endl;
		for (int i = 0; i < oldRegisters.size(); ++i)
			fout << "R[" << i << "]=" << oldRegisters[i] << " ";
		fout << endl;
		for (int i = 0; i < registers.size(); ++i)
			fout << "R[" << i << "]=" << registers[i] << " ";*/

		//fout << endl;

		if (iP == 28)
		{
			if ( registers[3] > max_v )
				{
					max_v = registers[3];
					cout << max_v << endl;
				}
		}


		if (iP >= instructions.size())
			break;

		//tick++;
		/*if (tick % 1'000'000'000 == 0)
		{

			fout << "OiP: " << oldIp << "   | ";
			for (int i = 0; i < oldRegisters.size(); ++i)
				fout << oldRegisters[i] << " ";
			fout << endl;
			fout << "IP: " << iP << "    ";
			for (int i = 0; i < registers.size(); ++i)
				fout << registers[i] << " ";
			fout << endl;
			tick = 0;
			fout << endl;
			cout << "TOCK" << endl;
		}*/

		/*if (registers[4] != oldRegisters[4])
		{
			fout << registers[4] << " " << registers[3] << endl;

			cout << registers[4] << " " << registers[3] << endl;
		}*/


		/*if (registers[0] != oldRegisters[0])
		{
			for (auto & iPC : history)
				fout << iPC << endl;
			fout << "REG CHANGE!" << endl;

			fout << "OLD IP: " << oldIp << "    ";
			for (int i = 0; i < oldRegisters.size(); ++i)
				fout << oldRegisters[i] << " ";
			fout << endl;

			fout << "NEW IP: " << iP << "    ";
			for (int i = 0; i < registers.size(); ++i)
				fout << registers[i] << " ";
			fout << endl;

			fout << "INSTR: O:" << (int)i.opcode << " A:" << i.registerA << " B:" << i.registerB << " C:" << i.registerC << endl;

			fout << "------" << endl;
			fout.flush();

			cout << "TICK" << endl;
		}*/

		/*if (history.size() > 15)
			history.pop_front();*/
	}

	for (int i = 0; i < registers.size(); ++i)
		cout << registers[i] << " ";
	cout << endl;

	cin.get();

	return 0;
}
