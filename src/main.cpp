#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <unordered_set>
#include <string>

#include "randomizedSelect.h"

int main(int argc, char** argv) {
  if (argc != 5)
    return -1;

  unsigned int setSize = std::stoul(argv[1]);
  const unsigned int selectionCut = std::stoul(argv[2]);
  const std::string type(argv[3]);
  unsigned int numberOfThreads = std::stoul(argv[4]);

  /*
  // O vetor deve ser elementos únicos, seria melhor usar o set,
  // mas teria que colocar os elementos aleatórios nele.
  std::vector<int> setA;
  std::uniform_int_distribution<int> distribution(
      0, std::numeric_limits<int>::max());
  std::mt19937 engine;  // Mersenne twister MT19937
  auto generator = std::bind(distribution, engine);
  std::generate_n(setA.begin(), setSize, generator);
  */
  std::unordered_set<int> setA;
  setA.reserve(setSize);
  std::random_device rd;
  std::mt19937 engine(rd());  // Mersenne twister MT19937
  std::uniform_int_distribution<int> distribution(
    0, std::numeric_limits<int>::max());
  
  while (setA.size() != setSize) {
    setA.insert(distribution(engine));
  }

  std::vector<int> vecA(setA.begin(), setA.end());
  setA.~unordered_set();
  std::cout << "Vector generated: ";
  for (const int a : vecA) {
    std::cout << a << " ";
  }
  std::cout << std::endl;

  const auto startTime = std::chrono::high_resolution_clock::now();

  const int value = randomizedSelect(vecA, 0, setSize - 1, selectionCut, numberOfThreads);

  const auto endTime = std::chrono::high_resolution_clock::now();

  std::cout << "Result:" << value << std::endl;

  // TODO Imprimir ith número em A, o tempo de execução em segundos, e os
  // valores de A.

  return 0;
}