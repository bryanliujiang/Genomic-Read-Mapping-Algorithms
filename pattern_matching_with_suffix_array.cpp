#include "pattern_matching_with_suffix_array.h"
using namespace std;

int minIndex, maxIndex, midIndex, first, last; // global variables involved (made sure no duplicates)

std::pair<int, int> PatternMatchingWithSuffixArray(const std::string& Text, const std::string& Pattern, const int* SuffixArray)
{
    minIndex = 0; // minIndex <- 0
    maxIndex = Text.size() - 1; // maxIndex <- |Text| - 1
    while (minIndex <= maxIndex)
    {
        midIndex = (minIndex + maxIndex) / 2; // midIndex <- floor((minIndex + maxIndex)/2)
        if (Pattern > Text.substr(SuffixArray[midIndex])) // Pattern > suffix of Text starting at position SuffixArray(midIndex)
            minIndex = midIndex + 1; // minIndex <- midIndex + 1
        else
            maxIndex = midIndex - 1; // maxIndex <- midIndex - 1
    }
    if (Pattern == Text.substr(SuffixArray[midIndex], Pattern.size())) // Pattern matches a suffix of Text if the first |Pattern| symbols of this suffix match Pattern
        first = midIndex; // first <- midIndex
    else
        return make_pair(-1, -1); // Pattern does not appear in Text
    minIndex = first; // minIndex <- first
    maxIndex = Text.size() - 1; // maxIndex <- |Text| - 1
    while (minIndex <= maxIndex)
    {
        midIndex = (minIndex + maxIndex) / 2; // midIndex <- floor((minIndex + maxIndex)/2)
        if (Pattern > Text.substr(SuffixArray[midIndex])) //Pattern > suffix of Text starting at position SuffixArray(midIndex)
            minIndex = midIndex + 1; // minIndex <- midIndex + 1
        else
            maxIndex = midIndex - 1; // maxIndex <- midIndex - 1
    }
    last = maxIndex; // last <- maxIndex
    return make_pair(first, last); // return (first, last)
}