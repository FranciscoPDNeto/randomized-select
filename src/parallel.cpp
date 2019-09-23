#include <pthread.h>
#include "partition.h"

std::vector<int> prefixSum(const std::vector<int> mask, bool invert = false);

int partition(std::vector<int>& setA,
              const int partBeginning,
              const int partEnding,
              const unsigned int numberThreads) {
  int pivot = setA[partEnding];

  // parallel
  std::vector<int> mask(partEnding - partBeginning + 1);
  for (int i = 0; i <= partEnding - partBeginning; i++)
    mask[i] = setA[i + partBeginning] <= pivot ? 1 : 0;

  std::vector<int> lessPrefixSum = prefixSum(mask);
  std::vector<int> greaterPrefixSum = prefixSum(mask, true);

  // parallel
  std::vector<int> buffer(&setA[partBeginning], &setA[partEnding] + 1);
  int greaterBeginning = partBeginning + lessPrefixSum.back();
  for (int i = 0; i <= partEnding - partBeginning; i++) {
    if (mask[i])
      setA[partBeginning + lessPrefixSum[i] - 1] = buffer[i];
    else
      setA[greaterBeginning + greaterPrefixSum[i] - 1] = buffer[i];
  }
  return greaterBeginning - 1;
}

std::vector<int> prefixSum(const std::vector<int> mask, bool invert) {
  // parallel
  std::vector<int> sum(mask);
  if (invert)
    sum[0] = !sum[0];
  for (std::vector<int>::size_type i = 1; i < sum.size(); i++)
    sum[i] = sum[i - 1] + (invert ? !sum[i] : sum[i]);
  return sum;
}