#pragma once
#include "LoxCallable.h"
#include "Environment.h"
#include <vector>
#include <any>
#include <string>

class LoxFunction : public LoxCallable {
private:
  Environment closure_;
public:
  LoxFunction(Function* declaration, Environment closure);

  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
  int arity();
  std::string ToString();

private:
  Function* declaration_;
};

