#pragma once
#include "LoxClass.h"
#include <unordered_map>


class LoxInstance {
 
  LoxClass* klass_;
  std::unordered_map<std::string, std::any> fields_;

  public:
    LoxInstance(LoxClass* klass);
    std::string toString();
    std::any get(Token* name);
    void set(Token* name, std::any value);

};