/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 14:51:27
# Desc   : 第 5 章 智能指针与内存管理
########################################################################
*/

#include <memory>

// 5.1 RAII 与引用计数

// cpp11 没有 make_unique
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {

  return 0;
}
