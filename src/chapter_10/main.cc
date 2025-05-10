/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 23:34:59
# Desc   : 第 10 章 展望: C++20 简介
########################################################################
*/

#include "cpp_utils/util.h"
#include <algorithm>
#include <list>

// 概念与约束 concepts and requires

void run_sort() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::list<int> l = {1, 2, 3};
  // sort 需要接受随机迭代器 random_iterator
  // std::sort(l.begin(), l.end());
  std::println();
}

int main() {
  run_sort();
  return 0;
}


