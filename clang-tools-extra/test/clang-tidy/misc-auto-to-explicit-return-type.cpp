// RUN: %check_clang_tidy %s misc-auto-to-explicit-return-type %t

template <typename T, typename U>
struct root_class {
};

template <typename U>
using typed_class = root_class<int, U>;

// FIXME: Add something that triggers the check here.
auto operator"" _some(long double) {
  return typed_class<long double>{};
}
// CHECK-MESSAGES: :[[@LINE-1]]:6: warning: function 'f' is insufficiently awesome [misc-auto-to-explicit-return-type]

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
// CHECK-FIXES: {{^}}void awesome_f();{{$}}

// FIXME: Add something that doesn't trigger the check here.
typed_class<long double> operator"" _some_more(long double) {
  return typed_class<long double>{};
};
