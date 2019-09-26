#include <pthread.h>
#include <iostream>
#include "PoolThread.h"
#include "partition.h"

std::vector<int> prefixSum(const std::vector<int> mask, bool invert = false);

class SetMaskParameters {
 public:
  std::vector<int>& setA;
  std::vector<int>& mask;
  int partBeginning;
  int pivot;
  int i;
  SetMaskParameters(std::vector<int>& setA,
                    std::vector<int>& mask,
                    int partBeginning,
                    int pivot,
                    int i)
      : setA(setA),
        mask(mask),
        partBeginning(partBeginning),
        pivot(pivot),
        i(i) {}
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

  SwapSetValuesParameters(std::vector<int>& setA,
                          std::vector<int>& mask,
                          std::vector<int>& lessPrefixSum,
                          std::vector<int>& greaterPrefixSum,
                          std::vector<int>& buffer,
                          int partBeginning,
                          int greaterBeginning,
                          int i)
      : setA(setA),
        mask(mask),
        lessPrefixSum(lessPrefixSum),
        greaterPrefixSum(greaterPrefixSum),
        buffer(buffer),
        partBeginning(partBeginning),
        greaterBeginning(greaterBeginning),
        i(i) {}

};
void swapSetValues(void* parameters) {
  SwapSetValuesParameters* p = (SwapSetValuesParameters*)parameters;
  if (p->mask[p->i])
    p->setA[p->partBeginning + p->lessPrefixSum[p->i] - 1] = p->buffer[p->i];
  else
    p->setA[p->greaterBeginning + p->greaterPrefixSum[p->i] - 1] = p->buffer[p->i];
}

void setMask(void* parameters) {
  SetMaskParameters* p = (SetMaskParameters*)parameters;
  p->mask[p->i] = p->setA[p->i + p->partBeginning] <= p->pivot ? 1 : 0;
  delete p;
}

int partition(std::vector<int>& setA,
              const int partBeginning,
              const int partEnding,
              const unsigned int numberThreads) {
  int pivot = setA[partEnding];

  PoolThread& pool = PoolThread::getInstance(numberThreads);

  std::vector<int> mask(partEnding - partBeginning + 1);
  for (int i = 0; i <= partEnding - partBeginning; i++) {
    SetMaskParameters* parameters =
        new SetMaskParameters(setA, mask, partBeginning, pivot, i);
    pool.addTask(new Task(&setMask, (void*)parameters));
  }
  pool.wait();

  std::vector<int> lessPrefixSum = prefixSum(mask);
  std::vector<int> greaterPrefixSum = prefixSum(mask, true);

  std::vector<int> buffer(&setA[partBeginning], &setA[partEnding] + 1);
  int greaterBeginning = partBeginning + lessPrefixSum.back();
  for (int i = 0; i <= partEnding - partBeginning; i++) {
    SwapSetValuesParameters* parameters = 
      new SwapSetValuesParameters(setA, mask, lessPrefixSum, 
        greaterPrefixSum, buffer, partBeginning, greaterBeginning, i);
    pool.addTask(new Task(&swapSetValues, (void*)parameters));
  }
  pool.wait();
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