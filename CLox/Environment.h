#pragma once
#include <unordered_map>
#include <any>
#include <iostream>
#include "Token.h"
class Environment
{

  public:
    std::unordered_map<std::string, std::any>* values_;
    Environment* enclosing_;
    Environment();
    Environment(Environment* enclosing);
    void define(std::string, std::any value);
    void assign(Token* name, std::any value);
    std::any get(Token* name);
    std::any getAt(int distance, std::string name);
    Environment* ancestor(int distance);
    void assignAt(int distance, Token* name, std::any value);
};

