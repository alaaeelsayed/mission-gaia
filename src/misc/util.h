#include <random>
#ifndef UTIL_H
#define UTIL_H

int randomNum(int lowerBound, int upperBound)
{
    // Generates random number in range. Includes lower bound and upper bound
    return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

#endif
