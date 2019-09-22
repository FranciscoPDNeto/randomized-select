#include "randomizedSelect.h"

int randomizedSelect(std::vector<int>& setA, const int partBegining, 
                     const int partEnding, const int elementIndex, const unsigned int numberThread) {

  if (partBegining == partEnding)
    return setA[partBegining];

  int pivotIndex = randomizedPartition(setA, partBegining, partEnding);
  int k = pivotIndex - partBegining + 1;
  if (elementIndex == k) 
    return setA[pivotIndex];
  else if (elementIndex < k) 
    return randomizedSelect(setA, partBegining, pivotIndex - 1, elementIndex, numberThread);
  else
    return randomizedSelect(setA, pivotIndex + 1, partEnding, elementIndex - k, numberThread);
  
}

int randomizedPartition(std::vector<int>& setA, const int p, const int r) {
  // modern cpp
  /*
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(p, r);
  int i = distr(eng);
  */
  // old style kk
  int i = p + rand() % ((r + 1) - p);
  std::swap(setA[i], setA[r]);
  return partition(setA, p, r);
}

int partition(std::vector<int>& setA, const int p, const int r) {
  int x = setA[r];
  int i = p - 1;
  for(int j = p; j < r; ++j) {
    if (setA[j] <= x) {
      i += 1;
      std::swap(setA[i], setA[j]);
    }
  }

  std::swap(setA[i + 1], setA[r]);
  return i + 1;
}