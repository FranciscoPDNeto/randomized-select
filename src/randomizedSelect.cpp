#include "randomizedSelect.h"
#include <algorithm>
#include "partition.h"

int randomizedSelect(std::vector<int>& setA,
                     const int partBeginning,
                     const int partEnding,
                     const int elementIndex,
                     const unsigned int numberThreads) {
  if (partBeginning == partEnding)
    return setA[partBeginning];

  int pivotIndex =
      randomizedPartition(setA, partBeginning, partEnding, numberThreads);
  int k = pivotIndex - partBeginning + 1;
  if (elementIndex == k)
    return setA[pivotIndex];
  else if (elementIndex < k)
    return randomizedSelect(setA, partBeginning, pivotIndex - 1, elementIndex,
                            numberThreads);
  else
    return randomizedSelect(setA, pivotIndex + 1, partEnding, elementIndex - k,
                            numberThreads);
}

int randomizedPartition(std::vector<int>& setA,
                        const int partBeginning,
                        const int partEnding,
                        const unsigned int numberThreads) {
  int pivotindex = partBeginning + rand() % ((partEnding + 1) - partBeginning);
  std::swap(setA[pivotindex], setA[partEnding]);
  return partition(setA, partBeginning, partEnding, numberThreads);
}