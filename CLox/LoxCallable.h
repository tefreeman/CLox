#pragma once
#include <any>
#include "Interpreter.h"
#include <vector>
#include <chrono>
class LoxCallable {
  public:
    virtual std::any Call(Interpreter* interpreter, std::vector<std::any> arguments) = 0;
    virtual int arity() = 0;
};


class LoxCallableClock : public LoxCallable {
public:
  int arity() {
    return 0;
  };
  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments) {
    auto start = std::chrono::system_clock::now();
    auto legacyStart = std::chrono::system_clock::to_time_t(start);
    return static_cast<double>(legacyStart);
  };
};