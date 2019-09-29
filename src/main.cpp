#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <string>
#include <unordered_set>

#include "randomizedSelect.h"

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "The program needs 5 arguments, just " << argc << " were given." << std::endl;
    return -1;
  }

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
  clock_t tListStart = clock();
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

  clock_t tStart = clock();

  const int value =
      randomizedSelect(vecA, 0, setSize - 1, selectionCut, numberOfThreads);

  clock_t tEnd = clock();

  if (type == "all") {
    for (const int a : vecA) {
      std::cout << a << " ";
    }
    std::cout << std::endl;
    std::cout << value << std::endl;
  }
  
  const double duration = double(tEnd - tStart)/CLOCKS_PER_SEC;
  const double durationWithListTime = double(tEnd - tListStart)/CLOCKS_PER_SEC;
  std::cout << "Without list generator time: " << std::setprecision(6) << duration << " seconds" << std::endl;
  std::cout << "With list generator time: " << std::setprecision(6) << durationWithListTime << " seconds" << std::endl;

  // TODO Imprimir ith número em A, o tempo de execução em segundos, e os
  // valores de A.

  return 0;
}