#include "partition.h"

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