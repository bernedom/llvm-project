// RUN: %check_clang_tidy %s misc-auto-to-explicit-return-type %t

template <typename T, typename U>
struct base_class {
};

template <typename U>
using typed_class = base_class<int, U>;

// FIXME: Add something that triggers the check here.
constexpr auto operator"" _some(long double) {
  return typed_class<long double>{};
}
// CHECK-MESSAGES: :[[@LINE-3]]:1: warning: operator '_some' returns auto instead of explicit return type [misc-auto-to-explicit-return-type]

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
// CHECK-FIXES: {{^}}void awesome_f();{{$}}

void any_f(){};

// FIXME: Add something that doesn't trigger the check here.
constexpr typed_class<long double> operator"" _some_more(long double) {
  return typed_class<long double>{};
};
