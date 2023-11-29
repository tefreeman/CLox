#pragma once
#include <any>
#include "expr.h"
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
    //virtual R visit(Class* expr) = 0;
    virtual R visit(Expression* expr) = 0;
    //virtual R visit(Function* expr) = 0;
    //virtual R visit(If* expr) = 0;
    virtual R visit(Print* expr) = 0;
    //virtual R visit(Return* expr) = 0;
    virtual R visit(Var* expr) = 0;
    //virtual R visit(While* expr) = 0;
  };
  virtual void accept(Visitor<void>* visitor) = 0;
};



class Expression : public Stmt {
public:
 Expr* expression_;
 Expression(Expr* expression);
 void accept(Visitor<void>* visitor);
};

class Print : public Stmt {
public:
  Expr* expression_;
  Print(Expr* expression);
  void accept(Visitor<void>* visitor);

};

class Var : public Stmt {
public:
  Token* name_;
  Expr* initalizer_;

  Var(Token* name, Expr* initalizer);

  void accept(Visitor<void>* visitor);
};

class Block : public Stmt {
  public:
    std::vector<Stmt*> statements_;

    Block(std::vector<Stmt*> statements);

    void accept(Visitor<void>* visitor);
};