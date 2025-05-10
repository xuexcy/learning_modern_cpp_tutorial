/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 15:01:29
# Desc   : 第 6 章 正则表达式
########################################################################
*/

#include <print>
#include <regex>
#include <string>

// std::regex_match
//  1. input: <std::string, std::regex>  output: bool
//  2. input: <std::string, std::smatch, std::regex>
int main() {
  std::string fnames[] = {"foo.txt", "bar.txt", "test", "a0.txt", "AAA.txt"};
  //  std::regex txt_regex("[a-z]+\\.txt");
  std::regex txt_regex(R"(([a-z]+)\.txt)");
  for (const auto& fname : fnames) {
    std::println("{}: {}", fname, std::regex_match(fname, txt_regex));
  }
  std::println();

  std::smatch match;  // 本质是 match_results，被定义为 std::match_results<std::string::const_iterator>>
  for (const auto& fname : fnames) {
    if (std::regex_match(fname, match, txt_regex)) {
      if (match.size() == 2) {
        auto s = match[1].str();
        std::println("sub-match[0]: {}", match[0].str());
        std::println("{} sub-match[1]: {}", fname, s);
      }
    }
  }
  return 0;
}


