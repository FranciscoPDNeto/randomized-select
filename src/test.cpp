#include <metacarpal.hpp>

#include "randomizedSelect.h"

int main() {
  Metacarpal::Producer p;

  {
    p.test("10 ordered numbers");
    std::vector<int> setA{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int first = randomizedSelect(setA, 0, setA.size() - 1, 1, 4);
    int middle = randomizedSelect(setA, 0, setA.size() - 1, 6, 4);
    int last = randomizedSelect(setA, 0, setA.size() - 1, 10, 4);
    p.equal(first, 0, "first should be 0");
    p.equal(middle, 5, "middle should be 5");
    p.equal(last, 9, "last should be 9");
  }

  {
    p.test("10 unordered numbers");
    std::vector<int> setA{1, 8, 6, 3, 7, 0, 4, 5, 9, 2};
    int first = randomizedSelect(setA, 0, setA.size() - 1, 1, 4);
    int middle = randomizedSelect(setA, 0, setA.size() - 1, 6, 4);
    int last = randomizedSelect(setA, 0, setA.size() - 1, 10, 4);
    p.equal(first, 0, "first should be 0");
    p.equal(middle, 5, "middle should be 5");
    p.equal(last, 9, "last should be 9");
  }

  return p.end();
}