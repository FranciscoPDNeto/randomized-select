#include "randomizedSelect.h"
#include <algorithm>
#include "partition.h"

int randomizedPartition(std::vector<int>& setA,
                        const int partBeginning,
                        const int partEnding,
                        const unsigned int numberThreads) {
  int pivotindex = partBeginning + rand() % ((partEnding + 1) - partBeginning);
  std::swap(setA[pivotindex], setA[partEnding]);
  return partition(setA, partBeginning, partEnding, numberThreads);
}

int partition(std::vector<int>& setA,
              const int partBeginning,
              const int partEnding,
              const unsigned int numberThreads) {
  int pivot = setA[partEnding];
  int i = partBeginning - 1;
  for (int j = partBeginning; j < partEnding; ++j) {
    if (setA[j] <= pivot) {
      i += 1;
      std::swap(setA[i], setA[j]);
    }
  }

  std::swap(setA[i + 1], setA[partEnding]);
  return i + 1;
}