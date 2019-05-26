// RUN: %check_clang_tidy %s misc-auto-to-explicit-return-type %t -- -- -std=c++17

template <typename T, typename U>
struct base_class {
};

template <typename U>
using typed_class = base_class<int, U>;

constexpr auto operator"" _intr(long double) {
  int x = 0;
  return x;
}
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: function '_intr' returns 'auto' instead of explicit return type 'int' [misc-auto-to-explicit-return-type]
// CHECK-FIXES: {{^}}replace with return type 'int'{{$}}

constexpr auto operator"" _some(long double) {
  return typed_class<long double>{};
}
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: operator '_some' returns 'auto' instead of explicit return type 'typed_class<long double>' [misc-auto-to-explicit-return-type]
// CHECK-FIXES: {{^}}replace with return type 'typed_class<long double>'{{$}}

// does not trigger
constexpr typed_class<long double> operator"" _some_more(long double) {
  return typed_class<long double>{};
};

void any_f(){};