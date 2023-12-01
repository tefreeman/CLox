#pragma once
#include <iostream>
#include "LoxCallable.h"
#include <unordered_map>
#include "LoxFunction.h"

class LoxClass: public LoxCallable {
  std::unordered_map<std::string, LoxFunction*> methods_;

  public:
    std::string name_;
    LoxClass(std::string& name, std::unordered_map<std::string, LoxFunction*> methods);
    std::string toString();
    LoxFunction* findMethod(std::string name);
    std::any Call(Interpreter* interpreter, std::vector<std::any> arguments);
    int arity();
};