/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 22:50:56
# Desc   : 第 9 章 其他杂项
########################################################################
*/

#include "cpp_utils/util.h"
#include <print>

// 9.1 新类型
// long long int: cpp11, 至少 64 bit

// 9.2 noexcept 的修饰和操作
void may_throw() {
  throw true;
}
auto non_block_throw = [] {
  may_throw();
};
void no_throw() noexcept  { // 如果抛出异常，则会 std::terminate()
}
auto block_throw = [] noexcept {
  no_throw();
};

// 上面的 noexcept 叫限定符, specifier, 指定是否抛出异常
// 下面的 noexcept 叫运算符, operator, 在编译校验声明是否抛出异常，如果表达式被声明为抛出异常，则返回 false
void run_throw() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("may_throw() noexcept: {}", noexcept(may_throw()));
  std::println("no_throw() noexcept: {}", noexcept(no_throw()));
  std::println("lambda_may_throw() noexcept: {}", noexcept(non_block_throw()));
  std::println("lambda_no_throw() noexcept: {}", noexcept(block_throw()));

  // noexcept 可以封锁异常，防止扩散
  try {
    may_throw();
  } catch(...) {
    std::println("exception from may_throw()");
  }
  try {
    non_block_throw();
  } catch(...) {
    std::println("exception from non_block_throw()");
  }
  try {
    block_throw();  // 没懂这里，理论上应该会调用 std::terminal()
  } catch(...) {
    std::println("exception from block_throw()");
  }
  std::println();
}

// 9.3 字面量
std::string s1 = "C:\\File\\To\\Path";
// 原始字符串字面量
std::string s2 = R"(C:\File\To\Path)";
// 自定义字面量
std::string operator"" _wow1(const char* wow1, size_t len) {
  return std::string(wow1) + "woooooooow, amazing";
}
std::string operator"" _wow2(unsigned long long i) {
  return std::to_string(i) + "woooow, amazing";
}
void run_literal() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("s1: {}", s1);
  std::println("s2: {}", s2);
  std::println("{}", "abc"_wow1);
  std::println("{}", 1_wow2);
}

// 9.4 内存对齐
struct Storage {
  char a;
  int b;
  double c;
  long long d;
};  // struct Storage
struct alignas(std::max_align_t) AlignasStorage {
  char a;
  int b;
  double c;
  long long d;
};  // struct AlignasStorage

void run_align() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::println("{}", alignof(Storage));
  std::println("{}", alignof(AlignasStorage));
  std::println();
}
int main() {
  run_throw();
  run_align();
  return 0;
}


