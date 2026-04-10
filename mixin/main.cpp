#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

class InstanceLimitExceeded : public std::exception {
  std::string msg_;

public:
  InstanceLimitExceeded(size_t limit) {
    msg_ = "Exceeded limit of : " + std::to_string(limit);
  }

  const char *what() const noexcept override { return msg_.c_str(); }
};

template <typename T, std::size_t Limit> struct InstanceLimiter {
  static std::size_t instances;

  InstanceLimiter() { inccheck(); }
  InstanceLimiter(const InstanceLimiter &) { inccheck(); }
  InstanceLimiter(InstanceLimiter &&) { inccheck(); }
  InstanceLimiter &operator=(const InstanceLimiter &other) noexcept = default;
  InstanceLimiter &operator=(InstanceLimiter &&) noexcept = default;

private:
  void inccheck() {
    ++instances;
    if (instances >= Limit) {
      --instances;
      throw InstanceLimitExceeded(Limit);
    }
  }

protected:
  ~InstanceLimiter() { --instances; }
};

template <typename T, std::size_t Lim>
std::size_t InstanceLimiter<T, Lim>::instances(0);

class Foo : InstanceLimiter<Foo, 3> {};
class Boo : InstanceLimiter<Boo, 4> {};

int main() {
  {
    Boo aa;
    std::cout << InstanceLimiter<Foo, 3>::instances; // 0
    std::cout << InstanceLimiter<Boo, 4>::instances; // 1
    Foo a;
    std::cout << InstanceLimiter<Foo, 3>::instances; // 1
    std::cout << InstanceLimiter<Boo, 4>::instances; // 1
    Foo b;
    Boo bb, cc;
    std::cout << InstanceLimiter<Foo, 3>::instances; // 2
    std::cout << InstanceLimiter<Boo, 4>::instances; // 3
  }
  Boo aa, bb, cc;
  std::cout << InstanceLimiter<Foo, 3>::instances; // 0
  std::cout << InstanceLimiter<Boo, 4>::instances; // 3
  Foo a, b;
  std::cout << InstanceLimiter<Foo, 3>::instances; // 2
  std::cout << InstanceLimiter<Boo, 4>::instances; // 3

  try {
    Foo c;
  } catch (const InstanceLimitExceeded &e) {
    std::cout << "e";
  }

  try {
    Foo c;
  } catch (const InstanceLimitExceeded &e) {
    std::cout << "e";
  }

  try {
    Boo d;
  } catch (const InstanceLimitExceeded &e) {
    std::cout << "e";
  }

  a = std::move(b);
  aa = std::move(bb);
  std::cout << InstanceLimiter<Foo, 3>::instances; // 2
  std::cout << InstanceLimiter<Boo, 4>::instances; // 3

  std::cout << std::endl;
  // shall output 0111230323eee23

  return 0;
}
