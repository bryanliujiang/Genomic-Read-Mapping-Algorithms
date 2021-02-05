#ifndef BETTER_BW_MATCHING_H_
#define BETTER_BW_MATCHING_H_

#include <string>

/* Algorithm */
int BetterBWMatching(const int* FirstOccurrence, const std::string& LastColumn, std::string& Pattern, const int (*Count)[5]);

/* Helper functions */
bool contains(const int& t, const int& b, const std::string& l, const int& s); // handles whether positions from top to bottom in LastColumn contain an occurrence of symbol
int index(char symbol); // converts nucleotide symbol to respective index for inputs (e.g. FirstOccurrence, Count)

#endif // BETTER_BW_MATCHING_H_