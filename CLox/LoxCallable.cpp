#pragma once
#include "LoxCallable.h"
#include "Interpreter.h"
#include <vector>
#include <chrono>
#include <any>

  int LoxCallableClock::arity() {
    return 0;
  };
  std::any LoxCallableClock::Call(Interpreter* interpreter, std::vector<std::any> arguments) {
    auto start = std::chrono::system_clock::now();
    auto legacyStart = std::chrono::system_clock::to_time_t(start);
    return static_cast<double>(legacyStart);
  };
