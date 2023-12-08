#pragma once
#include "LoxCallable.h"
#include "..\Environment.h"
#include <vector>
#include <any>
#include <string>

// TODO fix foward delcaration 
class LoxInstance;

class LoxFunction : public LoxCallable {
private:
  Environment* closure_;
  Function* declaration_;
  bool isInitializer_;

public:
  LoxFunction(Function* declaration, Environment* closure, bool isInitializer);

  std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
  int arity();
  std::string ToString();
  LoxFunction* bind(LoxInstance* instance);
};

