#pragma once

#include <vector>

int randomizedSelect(std::vector<int>& setA,
                     const int partBeginning,
                     const int partEnding,
                     const int elementIndex,
                     const unsigned int numberThreads);

int randomizedPartition(std::vector<int>& setA,
                        const int partBeginning,
                        const int partEnding,
                        const unsigned int numberThreads);