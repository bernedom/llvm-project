// RUN: %check_clang_tidy %s misc-auto-to-explicit-return-type %t -- -- -std=c++17

template <typename T, typename U>
struct base_class {

  constexpr base_class(U v) {}
};

template <typename U>
using typed_class = base_class<int, U>;

constexpr auto operator"" _intr(long double) {
  int x = 0;
  return x;
}
// CHECK-MESSAGES: :[[@LINE-4]]:11: warning: function 'operator""_intr' returns 'auto' instead of explict return type 'int' [misc-auto-to-explicit-return-type]
// CHECK-FIXES: constexpr int operator"" _intr(long double)

constexpr auto operator"" _some(long double x) {
  return typed_class<long double>{x};
}
// CHECK-MESSAGES: :[[@LINE-3]]:11: warning: function 'operator""_some' returns 'auto' instead of explict return type 'typed_class<long double>' [misc-auto-to-explicit-return-type]
// CHECK-FIXES: constexpr typed_class<long double> operator"" _some(long double x)

// does not trigger
constexpr typed_class<long double> operator"" _some_more(long double x) {
  return typed_class<long double>{x};
};

template <typename T>
constexpr auto _dependend_func(T x) {
  return typed_class<T>{x};
}

void any_f(){};