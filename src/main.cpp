#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <string>

#include "eratosthenes.h"

int main(int argc, char** argv) {
  if (argc != 4)
    return -1;

  unsigned int setSize = std::stoul(argv[1]);
  const unsigned int selectionCut = std::stoul(argv[2]);
  unsigned int numberOfThreads = std::stoul(argv[3]);

  // O vetor deve ser elementos únicos, seria melhor usar o set,
  // mas teria que colocar os elementos aleatórios nele.
  std::vector<int> setA;    
  std::uniform_int_distribution<int> distribution(0, (int)(std::numeric_limits<int>::max));
  std::mt19937 engine; // Mersenne twister MT19937
  auto generator = std::bind(distribution, engine);
  std::generate_n(setA.begin(), setSize, generator); 

  const auto startTime = std::chrono::high_resolution_clock::now();

  // TODO chamar Calculate Selection.

  const auto endTime = std::chrono::high_resolution_clock::now();

  // TODO Imprimir ith núermo em A, o tempo de execução em segundos, e os valores de A.

  return 0;
}