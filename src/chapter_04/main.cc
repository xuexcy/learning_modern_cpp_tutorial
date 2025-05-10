/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 13:43:04
# Desc   : 第 4 章 容器
########################################################################
*/

#include <print>
#include <stdexcept>
#include <tuple>
#include <variant>

#include "cpp_utils/util.h"

// 4.1 线性容器

// 4.3 元组
template <size_t n, typename... T>
constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i) {
  if constexpr (n >= sizeof...(T)) {
    throw std::out_of_range("越界");
  }
  if (i == n) {
    return std::variant<T...>{std::in_place_index<n>, std::get<n>(tpl)};
  }
  // 虽然我们需要的是 n+1，但如果直接写成 n+1, 就会一直递归实例化，直到越界
  // return _tuple_index<n+1>(tpl, i);

  if constexpr (n < sizeof...(T) - 1) {
    return _tuple_index<n+1>(tpl, i);
  } else {
    // 为了在实例化时不越界,我们可以在条件导致越界时，随便调用一个一个实例
    // 这段代码看起来又讲调用重复循环了，但实际上只是终止了实例化，
    return _tuple_index<0>(tpl, i);
  }
}
/* 假设sizeof...(T) = 4
当 n = 0 时的实例化结果
if (i == n) {
  return ...
}
return _tuple_index<1>(tpl, i);

当 n = 1 时的实例化结果
if (i == n) {
}
return _tuple_index<2>(tpl, i);

当 n = 2 时的实例化结果
if (i == n) {
}
return _tuple_index<3>(tpl, i);

当 n = 3 时的实例化结果
if (i == n) {
}
return _tuple_index<0>(tpl, i);  // 虽然这里发生了循环，但实际上并不会调用，因为前面的 i == n 一定会发生

*/

template <typename... T>
constexpr std::variant<T...> tuple_index(const std::tuple<T...>& tpl, size_t i) {
  return _tuple_index<0>(tpl, i);
}

class A {
  int a_{0};
public:
  A(int a) : a_(a) {}
  A(const A& other) {
    PRINT_CURRENT_FUNCTION_NAME;
  }
  auto value() const { return a_; }
};  // class A

// must in namespace std
template <>
struct std::formatter<A> : std::formatter<std::string> {
  auto format(const A& t, format_context& ctx) const {
    return formatter<string>::format(
      std::format("{}", t.value()), ctx);
  }
};

void run_tuple() {
  PRINT_CURRENT_FUNCTION_NAME;
  auto p = std::make_tuple(1, 'A', "BC");
  std::println("ID: {}", std::get<0>(p));
  std::println("grade: {}", std::get<char>(p));
  auto& [id, grade, name] = p;

  auto q = std::make_tuple(1.0, A(9));
  auto new_tuple = std::tuple_cat(p, q);
  for (size_t i = 0; i < std::tuple_size<decltype(new_tuple)>::value; ++i) {
    std::println("{}", i);
    std::println("{}", std::visit([](auto&& arg) { return std::format("{}", arg); }, tuple_index(new_tuple, i)));
  }
  std::println();
}

int main() {
  run_tuple();
  return 0;
}


