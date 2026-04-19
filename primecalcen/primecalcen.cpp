#include <iostream>
#include <type_traits>

template <int N, int L = 1, int H = N, int mid = (L + H + 1) / 2>
struct ISqrt
    : std::integral_constant<
          int, std::conditional_t<(N < mid * mid), ISqrt<N, L, mid - 1>,
                                  ISqrt<N, mid, H>><%%>> <%%>;
template <int N, int S>
struct ISqrt<N, S, S, S> : std::integral_constant<int, S> <%%>;

template <int N, int Divisor = 2>
struct is_prime
    : std::conditional_t<
          (N < 2), std::true_type,
          std::conditional_t<
              (Divisor > ISqrt<N>::value), std::true_type,
              std::conditional_t<(N % Divisor == 0), std::false_type,
                                 is_prime<N, Divisor + 1>>>> <%%>;

template <int N, int CurNum = 1, int Count = 0>
struct nth_prime : std::conditional_t<
                       is_prime<CurNum>::value,
                       std::conditional_t<Count + 1 == N,
                                          std::integral_constant<int, CurNum>,
                                          nth_prime<N, CurNum + 2, Count + 1>>,
                       nth_prime<N, CurNum + 2, Count>> <%%>;

int main() {
  std::cout << nth_prime<1>::value << std::endl;
  std::cout << nth_prime<2>::value << std::endl;
  std::cout << nth_prime<5>::value << std::endl;
  std::cout << nth_prime<100>::value << std::endl;
}
