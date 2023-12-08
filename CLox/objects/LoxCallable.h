#pragma once
#include <any>
#include "..\Interpreter.h"
#include <vector>
class LoxCallable {
  public:
    virtual std::any Call(Interpreter* interpreter, std::vector<std::any> arguments) = 0;
    virtual int arity() = 0;
};


// Global _clock()
class LoxCallableClock : public LoxCallable {
public:
  int arity();
  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
  std::string toString();

};


// Global _console(int) -- returns console history at index
class LoxCallableConsoleHistory : public LoxCallable {
public:
  int arity();
  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
};