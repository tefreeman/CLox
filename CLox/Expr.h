#pragma once
#include <memory>
#include <iostream>
#include <any>
#include "Token.h"

class Assign;
class Binary;
class Grouping;
class Unary;
class Literal;
class Variable;

class Expr
{
public:
  template<typename R>
  class Visitor {
  public:
    virtual R visit(Binary* expr) = 0;
    virtual R visit(Grouping* expr) = 0;
    virtual R visit(Unary* expr) = 0;
    virtual R visit(Literal* expr) = 0;
    virtual R visit(Variable* expr) = 0;
    virtual R visit(Assign* expr) = 0;
   
    //virtual R visit(Call* expr) = 0;
    //virtual R visit(Get* expr) = 0;
    //virtual R visit(Logical* expr) = 0;
    //virtual R visit(Set* expr) = 0;
    //virtual R visit(Super* expr) = 0;
    //virtual R visit(This* expr) = 0;
  };
    virtual std::any accept(Visitor<std::any>* visitor) = 0;
};


class Unary : public Expr {
  public:
    Token* op_;
    Expr* right_;

    Unary(Token* op, Expr* right) {
      this->op_ = op;
      this->right_ = right;
    }

    std::any accept(Visitor<std::any>* visitor) {
      return visitor->visit(this);
    }

};

class Binary : public Expr {
public:
  Expr* left_;
  Token* op_;
  Expr* right_;

  Binary(Expr* left, Token* op, Expr* right) {
    this->op_ = op;
    this->right_ = right;
    this->left_ = left;
  }

  std::any accept(Visitor<std::any>* visitor) {
    return visitor->visit(this);
  }

};

class Grouping : public Expr {
public:
  Expr* expression_;

  Grouping(Expr* expression) {
    expression_ = expression;
  }

  std::any accept(Visitor<std::any>* visitor) {
    return visitor->visit(this);
  }

};

class Literal : public Expr {
public:
  std::any value_;

  Literal(std::any value) {
    value_ = value;
  }

  std::any accept(Visitor<std::any>* visitor) {
    return visitor->visit(this);
  }

};

class Variable : public Expr {
  public:
    Token* name_;

    Variable(Token* name) {
      name_ = name;
    }

    std::any accept(Visitor<std::any>* visitor) {
      return visitor->visit(this);
    }

};

class Assign : public Expr {
  public:
    Token* name_;
    Expr* value_;

    Assign(Token* name, Expr* value) {
      name_ = name;
      value_ = value;
    }

    std::any accept(Visitor<std::any>* visitor) {
      return visitor->visit(this);
    }
};
