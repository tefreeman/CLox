#pragma once
#include "Token.h"
#include "Expr.h"
#include <vector>
#include "LoxError.h"
using namespace lox_types;

class Parser
{
public: 

  Parser(std::vector<Token> tokens) {
    tokens_ = tokens;
  }

  Expr* parse() {
    try {
      return expression();
    }
    catch (lox_error::ParseError error) {
      error.display();
      return nullptr;
    }
  }

private:
  std::vector<Token> tokens_;
  int current_ = 0;


  Expr* expression() {
    return equality();
 }

  Expr* equality() {
    Expr* expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
      Token* op = previous();
      Expr* right = comparison();
      expr = new Binary(expr, op, right);
    }

    return expr;
  }

  Expr* comparison() {
    Expr* expr = term();

    while (match(std::vector<TokenType>{GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
      Token* op = previous();
      Expr* right = term();
      expr = new Binary(expr, op, right);
    }

    return expr;
  }

  Expr* term() {
  Expr* expr = factor();
    while (match(std::vector<TokenType>{MINUS, PLUS})) {
      Token* op = previous();
      Expr* right = factor();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expr* factor() {
    Expr* expr = unary();

    while (match(SLASH, STAR)) {
      Token* op = previous();
      Expr* right = unary();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expr* unary() {
    if (match(BANG, MINUS)) {
      Token* op = previous();
      Expr* right = unary();
      return new Unary(op, right);
    }
    return primary();
  }

  Expr* primary() {
    if(match(FALSE)) return new Literal(false);
    if (match(TRUE)) return new Literal(true);
    if (match(NIL)) return new Literal(nullptr);

    if (match(NUMBER, STRING)) return new Literal(previous()->literal_);

    if (match(LEFT_PAREN)) {
      Expr* expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return new Grouping(expr);
    }

    throw lox_error::ParseError(peek(), "Expect expression.");

  }

  Token* consume(TokenType type, const char* message) {
    if(check(type)) return advance();

    throw lox_error::ParseError(peek(), message);
  }

  bool match(std::vector<TokenType> types) {
    for (const auto& type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }

    return false;
  }

  bool match(TokenType type) {
      if (check(type)) {
        advance();
        return true;
   }
    return false;
  }

  bool match(TokenType type, TokenType type2) {
    if (check(type) || check(type2)) {
      advance();
      return true;
    }
    return false;
  }

  bool check(lox_types::TokenType token_type) {
    if(isAtEnd()) return false;
    return peek()->type_ == token_type;
  }

  void synchronize() {
    advance();

    while (!isAtEnd()) {
      if(previous()->type_ == SEMICOLON) return;

      switch (peek()->type_) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
          return;
      }
      advance();
    }
  }
  Token* advance() {
    if (!isAtEnd()) {
      current_++;
    }
    return previous();
  }

  bool isAtEnd() {
    return peek()->type_ == EOF;
  }

  Token* peek() {
    return &tokens_.at(current_);
  }

  Token* previous() {
    return &tokens_.at(current_-1);
  }
};

