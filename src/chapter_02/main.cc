/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/08 23:44:28
# Desc   : 第 2 章 语言可用性的强化
########################################################################
*/

#include <_static_assert.h>
#include <initializer_list>
#include <print>
#include <type_traits>
#include <vector>

#include "cpp_utils/util.h"

// 2.1 常量

// nullptr
// https://github.com/xuexcy/learning_more_cpp_idioms/blob/main/src/nullptr.cc

void foo(char*) {
  PRINT_CURRENT_FUNCTION_NAME;
}
void foo(int) {
  PRINT_CURRENT_FUNCTION_NAME;
}

void run_nullptr() {
  static_assert(std::is_same_v<int, decltype(0)>);
  static_assert(std::is_same_v<long, decltype(NULL)>);
  static_assert(!std::is_same_v<decltype(NULL), decltype(0)>);
  static_assert(!std::is_same_v<decltype(NULL), decltype((void*)0)>);
  static_assert(!std::is_same_v<decltype(NULL), std::nullptr_t>);
  foo(0);  // foo(int)
  // 编译错误: ambiguous
  //  foo(NULL);
  foo(nullptr);  // foo(char*)
  std::println();
}


// constexpr: 在编译期确定是一个常量表达式
#define LEN 10
int len_foo() {
  int i = 2;
  return i;
}
constexpr int len_foo_constexpr() {
  return 5;
}

constexpr int fibonacci(const int n) {
  return n == 1 || n == 2 ? 1 : fibonacci(n-1) + fibonacci(n-2);
}
void run_constexpr() {
  char arr_1[10];
  char arr_2[LEN];
  int len = 10;
  // char arr_3[len];  // 应该是不行的，因为 len 的值应该在运行时决定，不过 clang 19.1.7 好像可以编译通过
  const int len_2 = len + 1;  // len_2 是常数，但不是常量表达式
  constexpr int len_2_constexpr = 1 + 2 + 3;
  // char arr_4[len_2];  // 应该是不行的
  char arr_5[len_2_constexpr];
  // char arr_6[len_foo() + 5];  // 应该是不行的
  char arr_7[len_foo_constexpr() + 1];
  // 编译时就可以确定 constexpr 的值
  static_assert(55 == fibonacci(10));
  std::println();
}

// 2.2 变量及其初始化

// if/switch 变量声明强化: c++17
void run_variable_declare_in_if() {
  std::vector<int> a;
  if (auto it = std::find(a.begin(), a.end(), 3); it != a.end()) {
    std::println("found 3");
  }
  std::println();
}
// 初始化列表: initialize_list
class MagicFoo {
  std::vector<int> vec;
public:
  // 初始化列表
  MagicFoo(std::initializer_list<int> list) {
    for (auto it = list.begin(); it != list.end(); ++it) {
      vec.push_back(*it);
    }
  }
};  // class MagicFoo

void run_initialize_list() {
  PRINT_CURRENT_FUNCTION_NAME;
  MagicFoo magic_foo = {1, 2, 3, 4, 5};
  MagicFoo magic_foo2{1, 2};
  std::println();
}

// 结构化绑定
std::tuple<int, double, std::string> f() {
  return std::make_tuple(1, 2.3, "456");
}

void run_structured_binding() {
  PRINT_CURRENT_FUNCTION_NAME;
  auto [x, y, z] = f();
  std::println("x={}, y={}, z={}", x, y, z);
  std::println();
}

// 2.3 类型推导
auto add_cpp14 = [](auto x, auto y) -> int {
  return x + y;
};
int add_cpp20(auto x, auto y) {
  PRINT_CURRENT_FUNCTION_NAME;
  return x + y;
}
void run_auto() {
  PRINT_CURRENT_FUNCTION_NAME;
  add_cpp20(1, 2);  // auto = int
  add_cpp20(1.0, 2.0);  // auto = double
  std::println();
}

// 尾返回类型(trailing return type)推导
template <typename R, typename T, typename U>
R add(T x, U y) {
  return x + y;
}
template <typename T, typename U>
auto add_cpp11(T x, U y) -> decltype(x+y) {
  return x + y;
}
template <typename T, typename U>
auto add_cpp14_func(T x, U y) {
  return x + y;
}

// decltype(auto): cpp14, 转换函数或封装的返回类型
std::string lookup1() { return "1"; };
std::string a{"b"};
std::string& lookup2() { return a; }
decltype(auto) look1() { return lookup1(); }
decltype(auto) look2() { return lookup2(); }

// 2.4 控制流
// if constexpr: cpp17
template <typename T>
auto print_type_info(const T& t) {
  PRINT_CURRENT_FUNCTION_NAME;
  if constexpr (std::is_integral_v<T>) {
    return t  + 1;
  } else {
    return t + 0.001;
  }
}
void run_if_constexpr() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("{}", print_type_info(5));
  std::println("{}", print_type_info(5.0));
  std::println();
}

// 2.5 模板
// 外部模板
// 每个编译单元在 #include 带有模板定义的头文件时，都会实例化，这样会导致重复的实例化增加编译时间
// cpp11 引入外部模板，可以显示通知编译器当前编译单元不用实例化
template class std::vector<bool>;  // 强行实例化
extern template class std::vector<double>;  // 不要在该编译单元中实例化

// 变参模板展开
template <typename T0, typename... T>
void my_print(T0 t0, T... t) {
  std::println("{}", t0);
  if constexpr (sizeof...(T) > 0) {
    my_print(t...);
  }
}
template <typename T0, typename... T>
auto my_print2(T0 t0, T... t) {
  std::println("{}", t0);
  // 通过初始化列表展开
  // 这里使用了逗号运算符，首先执行逗号前的 lambda 表达式，然后返回 t0 用于初始化列表
  // 其实跟 T0 和 t0 没有关系，
  // 所以不写 (void) 也可以
  (void) std::initializer_list<T0>{([&t] {
    std::println("{}", t);
  }(), t0)...};
}
void run_my_print() {
  PRINT_CURRENT_FUNCTION_NAME;
  my_print(1, "a", "b");
  my_print2(1, "a", "b");

}


// 折叠表达式
template <typename... T>
auto sum(T... t) {
  return (t + ...);
}

template <int value>
void print_a_value() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("{}", value);

}
// 非类型模板参数推导: cpp17
template <auto value>
void print_a_value2() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("{}", value);
}
void run_print_a_value() {
  print_a_value<1>();
  print_a_value2<1>();
  print_a_value2<'c'>();
  std::println();
}

// 委托构造函数: cpp11
// 一个构造函数调用另一个构造函数, 初始化列表中只能包含一个其他构造函数
class Base {
  int a{0};
  int b{0};
public:
  Base() = default;
  Base(int a_): Base() {
    a = a_;
  }
  Base(int a_, int b_): Base(a_) {
    b = b_;
  }
};  // class Base

// 继承构造: cpp11
// 通过 override 让编译器检测是否是继承的虚函数
class SubClass : public Base {
public:
  using Base::Base;  // 继承构造
};  // class SubClass : public Base

int main() {
  run_nullptr();
  run_constexpr();
  run_variable_declare_in_if();
  run_initialize_list();
  run_structured_binding();
  run_auto();
  run_if_constexpr();
  run_my_print();
  run_print_a_value();
  return 0;
}
