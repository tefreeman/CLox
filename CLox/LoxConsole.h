#pragma once
#include <iostream>
#include <vector>
#include <sstream>

namespace lox_console {
  inline std::vector<std::string> _console_history{};

  inline void _pushByNewline(const std::string& str) {
    if (_console_history.empty())
      _console_history.push_back("");

     if (str.find('\n') == std::string::npos) {
        // No newline characters found, add the entire string
        _console_history.back() += str;
        return;
        }

      std::istringstream ss(str);

      for (std::string line; std::getline(ss, line, '\n');) {
        _console_history.back() += line;
        _console_history.push_back("");
      }

  }
  inline void print(const std::string str) {
    std::cout << str;
    _pushByNewline(str);
  }

  inline void clear() {
    _console_history.clear();
  }

  inline std::string get(int lineNum) {
    return _console_history[lineNum];
  }
}