#include "better_bw_matching.h"

#include <iostream>
using namespace std;

int top, bottom, symbol; // global variables involved (made sure no duplicates)

int BetterBWMatching(const int* FirstOccurrence, const std::string& LastColumn, std::string& Pattern, const int(*Count)[5])
{

	top = 0; // top <- 0
	bottom = LastColumn.size() - 1; // bottom <- |LastColumn| - 1
	while (top <= bottom) // while top <= bottom
	{
		if (!Pattern.empty()) // if Pattern is nonempty
		{
			symbol = index(Pattern.back()); // symbol <- last letter in Pattern
			Pattern.pop_back(); // remove last letter from Pattern
			if (contains(top, bottom, LastColumn, symbol)) // if positions from top to bottom in LastColumn contain an occurrence of symbol
			{
				top = FirstOccurrence[symbol] + Count[top][symbol];
				bottom = FirstOccurrence[symbol] + Count[bottom + 1][symbol] - 1;
			}
			else
				return 0;
		}
		else
			return bottom - top + 1;
	}
}

bool contains(const int& t, const int& b, const std::string& l, const int& s)
{
	char nucleotide = 0;
	switch (s)
	{
	case 1:
		nucleotide = 'A';
		break;
	case 2:
		nucleotide = 'C';
		break;
	case 3:
		nucleotide = 'G';
		break;
	case 4:
		nucleotide = 'T';
		break;
	}

	for (int i = t; i <= b; ++i)
		if (l[i] == nucleotide)
			return true;

	return false;
}

int index(char symbol)
{
	switch (symbol)
	{
	case 'A':
		return 1;
	case 'C':
		return 2;
	case 'G':
		return 3;
	case 'T':
		return 4;
	default:
		cout << "Reads have invalid nucleotides! Must be 'A', 'C', 'G', or 'T'." << endl;
		exit(-1);
	}
}