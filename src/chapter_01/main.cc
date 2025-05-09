/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/08 22:41:44
# Desc   : 第一章 迈向现代 C++
########################################################################
*/

#include <print>

// 1.1 被弃用的特性

// 原文:如果一个类有析构函数，为其生成拷贝构造函数和拷贝赋值运算符的特性被弃用了
// 如果有析构函数，则可能在析构函数里面管理资源，如果没有合适的拷贝函数，则可能造成资源管理不当
// 根据 RAII 原则，设计一个管理资源的类时，必须明确如何获取和释放资源，这也包括拷贝函数
class MyClass {
  int* i{nullptr};
public:
  MyClass() : i(new int(1)) { }
  ~MyClass() { delete i; }
  // 拷贝构造函数和拷贝赋值运算符被弃用
};

void run_my_class() {
  MyClass obj1;
  // MyClass obj2 = obj1; // 错误：使用了本该弃用的拷贝构造函数, 造成 i 被 delete 两次
}

// 1.2 与 C 的兼容性

void run_compatible() {
  std::println("{}", __cplusplus);
}

int main() {
  run_my_class();
  return 0;
}
