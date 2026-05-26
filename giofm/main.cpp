#include <cstddef>
#include <iostream>
#include <utility>

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker check, Args... args) {
  std::size_t idx = 0;

  if (((check(std::forward<Args>(args)) ? true : (++idx, false)) || ...)) {
    return static_cast<int>(idx);
  }

  return -1;
}

int main(void) {
  std::cout << getIndexOfFirstMatch([](int a) { return false; }) << std::endl;
  std::cout << getIndexOfFirstMatch([](int a) { return a % 2 == 0; }, 1, 3, 4)
            << std::endl;
}
