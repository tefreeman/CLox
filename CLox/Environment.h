#pragma once
#include <unordered_map>
#include <any>
#include <iostream>
#include "Token.h"
class Environment
{
  private:
    std::unordered_map<std::string, std::any> values_;
  public:
    
    Environment* enclosing_;
    Environment();
    Environment(Environment* enclosing);
    void define(std::string, std::any value);
    void assign(Token* name, std::any value);
    std::any get(Token* name);
};

