
#include "Stmt.h"
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




  Expression::Expression(Expr* expression) {
    expression_ = expression;
  }

  void Expression::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }


  Print::Print(Expr * expression) {
    expression_ = expression;
  }

  void Print::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }


  Var::Var(Token* name, Expr* initalizer) {
    name_ = name;
    initalizer_ = initalizer;
  }

  void Var::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }

  Block::Block(std::vector<Stmt*> statements)
  {
    statements_ = statements;
  }

  void Block::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }

  If::If(Expr* condition, Stmt* thenBranch, Stmt* elseBranch)
  {
    condition_ = condition;
    thenBranch_ = thenBranch;
    elseBranch_ = elseBranch;
  }

  void If::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }

  While::While(Expr* condition, Stmt* body)
  {
    condition_ = condition;
    body_ = body;
  }

  void While::accept(Visitor<void>* visitor) {
    return visitor->visit(this);
  }


  Function::Function(Token* name, std::vector<Token*> params, std::vector<Stmt*> body)
  {
  name_ = name;
  params_ = params;
  body_ = body;
  }

  void Function::accept(Visitor<void>* visitor)
  {
    return visitor->visit(this);
  }
