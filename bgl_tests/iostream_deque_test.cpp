#include <deque>
#include <iostream>
#include <iterator>

int main(void) {
  std::deque<int> x;

  for (int i = 0; i < 10; ++i) {
    x.push_back(i);
  }

  for (auto i : x) {
    std::cout << i << std::endl;
  }

  return 0;
}