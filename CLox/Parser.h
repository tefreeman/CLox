#pragma once
#include <vector>
#include "Token.h"
#include "Stmt.h"
#include "Expr.h"
#include "LoxError.h"
#include "LoxTypes.h"

class Parser {
public:
  Parser(std::vector<Token> tokens);

  std::vector<Stmt*> parse();

private:
  std::vector<Token> tokens_;
  int current_ = 0;

  Stmt* declaration();
  Stmt* varDeclaration();
  Expr* expression();
  Stmt* statement();
  Function* function(std::string kind);
  Stmt* IfStatement();
  Stmt* PrintStatement();
  Stmt* WhileStatement();
  Stmt* ForStatement();
  Stmt* ExpressionStatement();
  Expr* equality();
  Expr* comparison();
  Expr* term();
  Expr* factor();
  Expr* unary();
  Expr* Call();
  Expr* FinishCall(Expr* callee);
  Expr* primary();
  Expr* assignment();
  Expr* ParseOr();
  Expr* ParseAnd();
  std::vector<Stmt*> block();
  Token* consume(lox_types::TokenType type, const char* message);
  bool match(std::vector<lox_types::TokenType> types);
  bool match(lox_types::TokenType type);
  bool match(lox_types::TokenType type, lox_types::TokenType type2);
  bool check(lox_types::TokenType token_type);
  void synchronize();
  Token* advance();
  bool isAtEnd();
  Token* peek();
  Token* previous();
};
