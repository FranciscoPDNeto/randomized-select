#include <pthread.h>
#include <iostream>
#include "PoolThread.h"
#include "partition.h"

std::vector<int> prefixSum(const std::vector<int> mask, bool invert = false);

struct SetMaskParameters {
  std::vector<int>& setA;
  std::vector<int>& mask;
  int partBeginning;
  int pivot;
  int i;
  int size;
  SetMaskParameters(std::vector<int>& setA,
                    std::vector<int>& mask,
                    int partBeginning,
                    int pivot,
                    int i,
                    int size)
      : setA(setA),
        mask(mask),
        partBeginning(partBeginning),
        pivot(pivot),
        i(i),
        size(size) {}
};
struct SwapSetValuesParameters {
  std::vector<int>& setA;
  std::vector<int>& mask;
  std::vector<int>& lessPrefixSum;
  std::vector<int>& greaterPrefixSum;
  std::vector<int>& buffer;
  int partBeginning;
  int greaterBeginning;
  int i;
  int size;
  SwapSetValuesParameters(std::vector<int>& setA,
                          std::vector<int>& mask,
                          std::vector<int>& lessPrefixSum,
                          std::vector<int>& greaterPrefixSum,
                          std::vector<int>& buffer,
                          int partBeginning,
                          int greaterBeginning,
                          int i,
                          int size)
      : setA(setA),
        mask(mask),
        lessPrefixSum(lessPrefixSum),
        greaterPrefixSum(greaterPrefixSum),
        buffer(buffer),
        partBeginning(partBeginning),
        greaterBeginning(greaterBeginning),
        i(i),
        size(size) {}
};
void swapSetValues(void* parameters) {
  SwapSetValuesParameters* p = (SwapSetValuesParameters*)parameters;
  for (int i = p->i; i < p->i + p->size; i++) {
    auto less = p->mask[i] * (p->partBeginning + p->lessPrefixSum[i]);
    auto greater = !p->mask[i] * (p->greaterBeginning + p->greaterPrefixSum[i]);
    p->setA[less + greater - 1] = p->buffer[i];
  }
}

void setMask(void* parameters) {
  SetMaskParameters* p = (SetMaskParameters*)parameters;
  for (int i = p->i; i < p->i + p->size; i++) {
    p->mask[i] = p->setA[i + p->partBeginning] <= p->pivot;
  }
  delete p;
}

int partition(std::vector<int>& setA,
              const int partBeginning,
              const int partEnding,
              const unsigned int numberThreads) {
  int pivot = setA[partEnding];

  PoolThread& pool = PoolThread::getInstance(numberThreads);

  const int granularity = 4 * numberThreads;

  const int maskSize = partEnding - partBeginning + 1;
  std::vector<int> mask(maskSize);
  const int sizePerTask = maskSize / granularity;
  const int lastTaskSize = sizePerTask + maskSize % granularity;

  int i;
  for (i = 0; i < granularity - 1; i++) {
    SetMaskParameters* parameters = new SetMaskParameters(
        setA, mask, partBeginning, pivot, i * sizePerTask, sizePerTask);
    pool.addTask(new Task(&setMask, (void*)parameters));
  }
  SetMaskParameters* parameters = new SetMaskParameters(
      setA, mask, partBeginning, pivot, i * sizePerTask, lastTaskSize);
  pool.addTask(new Task(&setMask, (void*)parameters));
  pool.wait();

  std::vector<int> lessPrefixSum = prefixSum(mask);
  std::vector<int> greaterPrefixSum = prefixSum(mask, true);

  std::vector<int> buffer(&setA[partBeginning], &setA[partEnding] + 1);
  int greaterBeginning = partBeginning + lessPrefixSum.back();

  for (int i = 0; i < granularity - 1; i++) {
    SwapSetValuesParameters* parameters = new SwapSetValuesParameters(
        setA, mask, lessPrefixSum, greaterPrefixSum, buffer, partBeginning,
        greaterBeginning, i * sizePerTask, sizePerTask);
    pool.addTask(new Task(&swapSetValues, (void*)parameters));
  }
  SwapSetValuesParameters* parametersSwap = new SwapSetValuesParameters(
      setA, mask, lessPrefixSum, greaterPrefixSum, buffer, partBeginning,
      greaterBeginning, i * sizePerTask, lastTaskSize);
  pool.addTask(new Task(&swapSetValues, (void*)parametersSwap));
  pool.wait();
  return greaterBeginning - 1;
}

std::vector<int> prefixSum(const std::vector<int> mask, bool invert) {
  std::vector<int> sum(mask);
  sum[0] ^= invert;
  for (std::vector<int>::size_type i = 1; i < sum.size(); i++)
    sum[i] = sum[i - 1] + (invert ^ sum[i]);
  return sum;
}