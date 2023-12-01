#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Interpreter.h"
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>
//TODO check Visitor<std::any> -- text says void
class Resolver: Expr::Visitor<void>, Stmt::Visitor<void>
{
private:

  enum class FunctionType {
    NONE,
    FUNCTION,
    INITIALIZER,
    METHOD
  };

  enum class ClassType {
    NONE,
    CLASS
  };
  Interpreter* interpreter_;
  std::deque<std::unordered_map<std::string, bool>> scopes;
  
  FunctionType currentFunction_;
  ClassType currentClass_;

  void resolve(Stmt* stmt);
  void resolve(Expr* expr);
  void beginScope();
  void endScope();
  void declare(Token* name);
  void define(Token* name);
  void resolveLocal(Expr* expr, Token* name);
  void resolveFunction(Function* func, FunctionType type);
public:
  void resolve(std::vector<Stmt*> statements);
  Resolver(Interpreter* interpreter);
  // Expr
  void visit(Variable* expr);
  void visit(Assign* expr);
  void visit(Binary* expr);
  void visit(CallExpr* expr);
  void visit(Grouping* expr);
  void visit(Literal* expr);
  void visit(Logical* expr);
  void visit(Unary* expr);
  void visit(Get* expr);
  void visit(Set* expr);
  void visit(This* expr);
  
  // Stmt
  void visit(Block* stmt);
  void visit(Var* stmt);
  void visit(Function* stmt);
  void visit(Expression* stmt);
  void visit(If* stmt);
  void visit(Print* stmt);
  void visit(Return* stmt);
  void visit(While* stmt);
  void visit(Class* stmt);


};

