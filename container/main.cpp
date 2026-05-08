#include <iostream>
#include <stdexcept>
#include <tuple>
#include <utility>

template <typename... Types> class Container {
  std::tuple<Types...> data;

  template <typename T, size_t First, size_t... Rest>
  T getElementByIndex(size_t idx) {
    if (idx == First) {
      return std::get<First>(data);
    }
    if constexpr (sizeof...(Rest) > 0) {
      return getElementByIndex<T, Rest...>(idx);
    } else {
      throw std::out_of_range("index out of range");
    }
  }

  template <typename T, size_t... Is>
  T getElementHelper(size_t idx, std::index_sequence<Is...>) {
    return getElementByIndex<T, Is...>(idx);
  }

public:
  Container(Types... args) : data(std::move(args)...) {}

  template <typename T> T getElement(size_t idx) {
    return getElementHelper<T>(idx, std::index_sequence_for<Types...>{});
  }
};

int main() {
  Container<int, char> c(12, 'c');
  std::cout << c.getElement<int>(0) << std::endl;
  std::cout << c.getElement<char>(1) << std::endl;
}
