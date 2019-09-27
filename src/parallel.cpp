#include <cmath>
#include <pthread.h>
#include <iostream>
#include "PoolThread.h"
#include "randomizedSelect.h"

struct RandomizedSelectParams {
  int rank;
  int numberElements;
  int numberProcessors;

  RandomizedSelectParams(const int rank, const int numberElements, const int numberProcessors) 
          : rank(rank), numberElements(numberElements), numberProcessors(numberProcessors) {}

};

void randomizedSelectTask(void *parameters) {
  const RandomizedSelectParams *randSelectParams = 
    (RandomizedSelectParams *) parameters;

  const int n = randSelectParams->numberElements;
  const int p = randSelectParams->numberProcessors;
  const int rank = randSelectParams->rank;
  int ni = n / p;
  while (n > std::max(std::pow(p, 2), 4096.0) && std::abs(n - rank) > 2*p) {
    //TODO implementation.

  }
}

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

