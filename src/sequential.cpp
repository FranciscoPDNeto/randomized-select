#include "randomizedSelect.h"

int randomizedSelect(std::vector<int>& setA,
                     const int partBeginning,
                     const int partEnding,
                     const int elementIndex,
                     const unsigned int numberThread) {
  if (partBeginning == partEnding)
    return setA[partBeginning];

  int pivotIndex = randomizedPartition(setA, partBeginning, partEnding);
  int k = pivotIndex - partBeginning + 1;
  if (elementIndex == k)
    return setA[pivotIndex];
  else if (elementIndex < k)
    return randomizedSelect(setA, partBeginning, pivotIndex - 1, elementIndex,
                            numberThread);
  else
    return randomizedSelect(setA, pivotIndex + 1, partEnding, elementIndex - k,
                            numberThread);
}

int randomizedPartition(std::vector<int>& setA,
                        const int partBeginning,
                        const int partEnding) {
  // modern cpp
  /*
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(p, r);
  int i = distr(eng);
  */
  // old style kk
  int pivotindex = partBeginning + rand() % ((partEnding + 1) - partBeginning);
  std::swap(setA[pivotindex], setA[partEnding]);
  return partition(setA, partBeginning, partEnding);
}

int partition(std::vector<int>& setA,
              const int partBeginning,
              const int partEnding) {
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