#include <cstddef>
#include <iostream>
#include <utility>

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker check, Args... args) {
  std::size_t idx = 0;
  bool found = false;
  ([&](auto &&arg) {
    if (found)
      return true;
    if (check(std::forward<decltype(arg)>(arg))) {
      found = true;
      return true;
    }
    ++idx;
    return false;
  }(std::forward<Args>(args)) ||
   ...);
  return found ? static_cast<int>(idx) : -1;
}

int main(void) {
  std::cout << getIndexOfFirstMatch([](int a) { return false; }) << std::endl;
  std::cout << getIndexOfFirstMatch([](int a) { return a % 2 == 0; }, 1, 3, 4)
            << std::endl;
}
