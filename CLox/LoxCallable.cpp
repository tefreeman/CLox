#pragma once
#include "LoxCallable.h"
#include "Interpreter.h"
#include <vector>
#include <chrono>
#include <any>
#include "LoxConsole.h"
#include "LoxError.h"

  int LoxCallableClock::arity() {
    return 0;
  };
  std::any LoxCallableClock::Call(Interpreter* interpreter, std::vector<std::any> arguments) {
    auto start = std::chrono::system_clock::now();
    auto legacyStart = std::chrono::system_clock::to_time_t(start);
    return static_cast<double>(legacyStart);
  }
  std::string LoxCallableClock::toString()
  {
    return "<native fn>";
  }
  ;

  int LoxCallableConsoleHistory::arity()
  {
    return 1;
  }


  std::any LoxCallableConsoleHistory::Call(Interpreter* interpreter, std::vector<std::any> arguments)
  {
    int pos = (int)std::any_cast<double>(arguments[0]);

    if (pos < 0 || pos >= lox_console::_console_history.size()) {
        return nullptr;
    }

    return lox_console::get(pos);
  }
