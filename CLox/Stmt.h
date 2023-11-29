#pragma once
#include <any>

class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;

class Stmt
{
public:
  template<typename R>
  class Visitor {
  public:
    virtual R visit(Block*) = 0;
    virtual R visit(Class* expr) = 0;
    virtual R visit(Expression* expr) = 0;
    virtual R visit(Function* expr) = 0;
    virtual R visit(If* expr) = 0;
    virtual R visit(Print* expr) = 0;
    virtual R visit(Return* expr) = 0;
    virtual R visit(Var* expr) = 0;
    virtual R visit(While* expr) = 0;
  };
  virtual std::any accept(Visitor<std::any>* visitor) = 0;
};
};

