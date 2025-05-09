/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/09 15:27:17
# Desc   : 第 3 章 语言运行期的强化
########################################################################
*/
#include <cstdlib>
#include <functional>
#include <memory>
#include <print>

// 3.1 Lambda 表达式
// 值捕获，在表达式被创建时就被捕获

// 表达式捕获: cpp14, 允许捕获任意的表达式进行初始化，允许右值捕获
void lambda_expression_capture() {
  auto important = std::make_unique<int>(1);
  // important 是一个 std::unique_ptr<int>，它是不能被捕获的，只能 move 走
  auto add = [v1 = 1, v2 = std::move(important)] (int x, int y) -> int {
    return x + y + v1 + (*v2);
  };
}

// 泛型 Lambda
auto add = [](auto x, auto y) {
  return x + y;
};

// 3.2 函数对象包装器
// std::function: cpp11
using foo = void(int);
void call_func(foo f) { f(1); }

// lambda 是一个闭包对象，当捕获列表为空时，可以转换为函数指针
void run_function() {
  // f 的捕获列表为空
  auto f = [](int v) { std::println("{}", v); };
  call_func(f);  // f 隐式转换为 foo* 类型的函数指针值
  f(1);


  int i = 1;
  auto f2 = [&](int v) { std::println("{}", i + v); };
  // f2 的捕获列表不为空，不能转换为 foo*
  // call_func(f2);

  std::function<void(int)> f3 = f;

  std::function<int(int)> f4 = [&](int v) -> int {
    return i + v;
  };
  // call_func(f4);
  std::println();
}
// std::bind 好像有性能问题, 主要是解决无法一次性捕获所有参数的问题
int add2(int a, int b, int c) {
  return a + b + c;
}
auto bind_add2 = std::bind(add2, std::placeholders::_1, 1, 2);


// 3.3 右值引用
// 字符串字面量是一个左值，所以可以进行引用
// "01234" 类型为 const char[6];
static_assert(std::is_same_v<decltype("01234"), const char(&)[6]>);
const char (&left)[6] = "01234";
// 数组可以被转换成相对应的指针类型，转换表达式的结果一定是个右值
const char* p = "01234";
const char*&& pr = "01234";

// 完美转发
void reference(int& v) {
  std::println("左值");
}
void reference(int&& v) {
  std::println("右值");
}
template <typename T>
void pass(T&& v) {
  std::println("普通传参");
  reference(v);  // v 作为一个变量，是一个左值，始终调用 reference(int&)
}
template <typename T>
void pass2(T&& v) {
  std::println("普通传参");
  reference(std::forward<T>(v));

}
void run_pass() {
  std::println("传递右值");
  pass(1);
  int l = 1;
  std::println("传递左值");
  pass(l);
}
int main() {
  run_function();
  run_pass();
  return 0;
}

