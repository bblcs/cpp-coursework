#include <concepts>
#include <cstddef>
#include <memory>

template <std::size_t SIZE, typename... Types>
  requires(std::copy_constructible<Types> && ...) && (sizeof...(Types) <= SIZE)
void allocate(void *memory, Types... args) {
  void *p = memory;
  std::size_t rem = SIZE;
  (
      [&]<typename T>(T &&arg) {
        std::align(alignof(T), sizeof(T), p, rem);
        new (p) T(std::forward<T>(arg));
        p = static_cast<std::byte *>(p) + sizeof(T);
        rem = SIZE -
              (static_cast<std::byte *>(p) - static_cast<std::byte *>(memory));
      }(args),
      ...);
}

int main(void) { return 0; }
