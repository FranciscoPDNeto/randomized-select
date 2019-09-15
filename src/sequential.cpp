#include "randomizedSelect.h"

int randomizedSelect(std::vector<int>& setA, const int p, 
                     const int r, const int i, const unsigned int numberThread) {

  if (p == r)
    return setA[p];

  int q = randomizedPartition(setA, p, r);
  int k = q - p + 1;
  if (i == k) 
    return setA[q];
  else if (i < k) 
    return randomizedSelect(setA, p, q - 1, i, numberThread);
  else
    return randomizedSelect(setA, q + 1, r, i - k, numberThread);
  
}

int randomizedPartition(std::vector<int>& setA, const int p, const int r) {
  int i = rand() % r + 1;
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