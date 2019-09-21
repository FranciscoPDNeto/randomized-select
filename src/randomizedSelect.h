#include <algorithm>
#include <vector>
#include <stdlib.h>

int randomizedSelect(std::vector<int>& setA, const int partBegining, 
                     const int partEnding, const int elementIndex, const unsigned int numberThread);

int randomizedPartition(std::vector<int>& setA, const int p, const int r);

int partition(std::vector<int>& setA, const int p, const int r);