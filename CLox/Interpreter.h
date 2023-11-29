#pragma once
#include "Expr.h"
#include <any>
class Interpreter : public Expr::Visitor<std::any>
{
private:
  std::any evaluate(Expr* expr);
  bool IsTruthy(std::any val);
  bool IsEqual(std::any val, std::any val2);
  void CheckNumberOperand(Token* op, std::any operand);
  void CheckNumberOperand(Token* op, std::any left, std::any right);
  std::string Stringify(std::any value);
public:
  Interpreter();
  void Interpret(Expr* expression);
  std::any visit(Literal*);
  std::any visit(Grouping*);
  std::any visit(Unary*);
  std::any visit(Binary*);
};

