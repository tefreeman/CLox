#pragma once
#include <any>
#include "Interpreter.h"
#include <vector>
class LoxCallable {
  public:
    virtual std::any Call(Interpreter* interpreter, std::vector<std::any> arguments) = 0;
    virtual int arity() = 0;
};


class LoxCallableClock : public LoxCallable {
public:
  int arity();
  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
};