#include <math.h>
#include <pthread.h>
#include <iostream>
#include "PoolThread.h"
#include "partition.h"

std::vector<int> prefixSum(const std::vector<int>& mask,
                           int granularity,
                           bool invert = false);

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
void setMask(void* parameters) {
  SetMaskParameters* p = (SetMaskParameters*)parameters;
  for (int i = p->i; i < p->i + p->size; i++) {
    p->mask[i] = p->setA[i + p->partBeginning] <= p->pivot;
  }
  delete p;
}

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
    auto less = p->mask[i] * (p->partBeginning + p->lessPrefixSum[i + 1]);
    auto greater =
        !p->mask[i] * (p->greaterBeginning + p->greaterPrefixSum[i + 1]);
    p->setA[less + greater - 1] = p->buffer[i];
  }
}

struct SweepParameters {
  std::vector<int>& scan;
  int i;
  int power;
  int size;
  SweepParameters(std::vector<int>& scan, int i, int power, int size)
      : scan(scan), i(i), power(power), size(size) {}
};
void upSweep(void* parameters) {
  SweepParameters* p = (SweepParameters*)parameters;
  for (int i = 0; i < p->size; i++) {
    int j = (p->i + i) * p->power;
    p->scan[j + p->power - 1] += p->scan[j + p->power / 2 - 1];
  }
  delete p;
}
void downSweep(void* parameters) {
  SweepParameters* p = (SweepParameters*)parameters;
  for (int i = 0; i < p->size; i++) {
    int j = (p->i + i) * p->power;
    int temp = p->scan[j + p->power / 2 - 1];
    p->scan[j + p->power / 2 - 1] = p->scan[j + p->power - 1];
    p->scan[j + p->power - 1] += temp;
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

  std::vector<int> lessPrefixSum = prefixSum(mask, granularity);
  std::vector<int> greaterPrefixSum = prefixSum(mask, granularity, true);

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

std::vector<int> prefixSum(const std::vector<int>& numbers,
                           int granularity,
                           bool invert) {
  PoolThread& pool = PoolThread::getInstance(4);

  std::vector<int> scan(&numbers[0], &numbers[numbers.size()]);
  if (invert)
    for (std::vector<int>::size_type i = 0; i < scan.size(); i++)
      scan[i] = !scan[i];
  scan.resize(pow(2, ceil(log2(numbers.size()))), 0);

  for (int d = 0; d < log2(scan.size()); d++) {
    int power = pow(2, d + 1);
    int tasks = scan.size() / power;
    int blockSize = tasks / granularity;
    int lastBlockSize = blockSize + tasks % granularity;
    int i;
    for (i = 0; i < granularity - 1; i++) {
      pool.addTask(new Task(&upSweep, new SweepParameters(scan, i * blockSize,
                                                          power, blockSize)));
    }
    pool.addTask(new Task(&upSweep, new SweepParameters(scan, i * blockSize,
                                                        power, lastBlockSize)));
    pool.wait();
  }
  int sum = scan[scan.size() - 1];
  scan[scan.size() - 1] = 0;
  for (int d = log2(scan.size()) - 1; d >= 0; d--) {
    int power = pow(2, d + 1);
    int tasks = scan.size() / power;
    int blockSize = tasks / granularity;
    int lastBlockSize = blockSize + tasks % granularity;
    int i;
    for (i = 0; i < granularity - 1; i++) {
      pool.addTask(new Task(&downSweep, new SweepParameters(scan, i * blockSize,
                                                            power, blockSize)));
    }
    pool.addTask(new Task(
        &downSweep,
        new SweepParameters(scan, i * blockSize, power, lastBlockSize)));
    pool.wait();
  }

  scan.push_back(sum);
  scan.resize(numbers.size() + 1);

  return scan;
}
