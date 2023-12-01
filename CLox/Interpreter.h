#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include <any>
#include <unordered_map>

class Interpreter : public Expr::Visitor<std::any>, public Stmt::Visitor<void>
{


private:
  Environment* environment_;
  std::unordered_map<Expr*, int> locals_;

  std::any evaluate(Expr* expr);
  bool IsTruthy(std::any val);
  bool IsEqual(std::any val, std::any val2);
  void CheckNumberOperand(Token* op, std::any operand);
  void CheckNumberOperand(Token* op, std::any left, std::any right);
  std::string Stringify(std::any value);
  void execute(Stmt* stmt);
  std::any lookUpVariable(Token* name, Expr* expr);

public:
  Environment* globals_ = new Environment();
  void executeBlock(std::vector<Stmt*> statements, Environment* env);
  Interpreter();
  void Interpret(std::vector<Stmt*> statements);
  void resolve(Expr* expr, int depth);
  std::any visit(Literal*);
  std::any visit(Grouping*);
  std::any visit(Unary*);
  std::any visit(Binary*);
  std::any visit(Variable*);
  std::any visit(Assign*);
  std::any visit(Logical*);
  std::any visit(CallExpr*);
  std::any visit(Get*);
  std::any visit(Set*);
  std::any visit(This*);
  std::any visit(Super*);

  // Stmt Visitor
  void visit(Expression*);
  void visit(Print*);
  void visit(Var*);
  void visit(Block*);
  void visit(If*);
  void visit(While*);
  void visit(Function*);
  void visit(Return*);
  void visit(Class*);
};

