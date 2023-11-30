#include "Parser.h"
#include "Token.h"
#include "Expr.h"
#include <vector>
#include "LoxError.h"
#include "Stmt.h"

using namespace lox_types;


Parser::Parser(std::vector<Token> tokens) {
    tokens_ = tokens;
  }

  std::vector<Stmt*> Parser::parse() {

    std::vector<Stmt*> statements;
    while (!isAtEnd()) {
      statements.push_back(declaration());
    }
    return statements;

  }



  Stmt* Parser::declaration() {
    try {
      if (match(VAR)) return varDeclaration();

      return statement();
    }
    catch (lox_error::ParseError err) {
      synchronize();
      return nullptr;
    }
  }


  Stmt* Parser::varDeclaration() {
    Token* name = consume(IDENTIFIER, "Expect variable name.");

    Expr* initalizer = nullptr;

    if (match(EQUAL)) {
      initalizer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration.");

    return new Var(name, initalizer);
  }

  Expr* Parser::expression() {
    return assignment();
  }

  Stmt* Parser::statement() {
    if (match(IF)) return IfStatement();
    if(match(WHILE)) return WhileStatement();
    if(match(FOR)) return ForStatement();
    if (match(PRINT)) return PrintStatement();
    
    if (match(LEFT_BRACE)) {
      return new Block(block());
    }
    return ExpressionStatement();
  }

  Stmt* Parser::IfStatement()
  {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt* thenBranch = statement();

    Stmt* elseBranch = nullptr;

    if (match(ELSE)) {
      elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
  }

  Stmt* Parser::PrintStatement() {
    Expr* value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Print(value);
  }

  Stmt* Parser::WhileStatement()
  {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr* condition = expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    Stmt* body = statement();

    return new While(condition, body);
  }

  Stmt* Parser::ForStatement()
  {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt* initializer;
    if (match(SEMICOLON)) {
      initializer = nullptr;
    }
    else if (match(VAR)) {
      initializer = varDeclaration();
    }
    else {
      initializer = ExpressionStatement();
    }


    Expr* condition = nullptr;
    if (!check(SEMICOLON)) {
      condition = expression();
    }

    consume(SEMICOLON, "Expect ';' after loop condition.");

    Expr* increment = nullptr;
    if (!check(RIGHT_PAREN)) {
      increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    
    Stmt* body = statement();



    if (increment != nullptr) {
      body = new Block(std::vector<Stmt*>{body, new Expression(increment)});
    }

    if (condition == nullptr) condition = new Literal(true);

    body = new While(condition, body);

    if (initializer != nullptr) {
      body = new Block(std::vector<Stmt*>{initializer, body});
    }
    return body;

  }

  Stmt* Parser::ExpressionStatement() {
    Expr* expr = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return new Expression(expr);
  }

  Expr* Parser::equality() {
    Expr* expr = comparison();

    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
      Token* op = previous();
      Expr* right = comparison();
      expr = new Binary(expr, op, right);
    }

    return expr;
  }

  Expr* Parser::comparison() {
    Expr* expr = term();

    while (match(std::vector<TokenType>{GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
      Token* op = previous();
      Expr* right = term();
      expr = new Binary(expr, op, right);
    }

    return expr;
  }

  Expr* Parser::term() {
    Expr* expr = factor();
    while (match(std::vector<TokenType>{MINUS, PLUS})) {
      Token* op = previous();
      Expr* right = factor();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expr* Parser::factor() {
    Expr* expr = unary();

    while (match(SLASH, STAR)) {
      Token* op = previous();
      Expr* right = unary();
      expr = new Binary(expr, op, right);
    }
    return expr;
  }

  Expr* Parser::unary() {
    if (match(BANG, MINUS)) {
      Token* op = previous();
      Expr* right = unary();
      return new Unary(op, right);
    }
    return primary();
  }

  Expr* Parser::primary() {
    if (match(FALSE)) return new Literal(false);
    if (match(TRUE)) return new Literal(true);
    if (match(NIL)) return new Literal(nullptr);

    if (match(NUMBER, STRING)) return new Literal(previous()->literal_);

    if (match(LEFT_PAREN)) {
      Expr* expr = expression();
      consume(RIGHT_PAREN, "Expect ')' after expression.");
      return new Grouping(expr);
    }

    if (match(IDENTIFIER)) {
      return new Variable(previous());
    }

    throw lox_error::ParseError(peek(), "Expect expression.");

  }

  Expr* Parser::assignment()
  {
    Expr* expr = ParseOr();

    if (match(EQUAL)) {
      Token* equals = previous();
      Expr* value = assignment();

      Variable* varCast = dynamic_cast<Variable*>(expr);
      if (varCast != nullptr) {
         Token* name = varCast->name_;
         return new Assign(name, value);
      }
      lox_error::Error(equals, "Invalid assignment target.");

    }
    return expr;
  }

  Expr* Parser::ParseOr()
  {
     Expr* expr = ParseAnd();

     while (match(OR)) {
      Token* op = previous();
      Expr* right = ParseAnd();
      expr = new Logical(expr, op, right);
     }

     return expr;
  }

  Expr* Parser::ParseAnd()
  {
    Expr* expr = equality();

    while (match(AND)) {
      Token* op = previous();
      Expr* right = equality();
      expr = new Logical(expr, op, right);
    }

    return expr;
  }

  std::vector<Stmt*> Parser::block()
  {
    std::vector<Stmt*> statements;

    while (!check(RIGHT_BRACE) && !isAtEnd()) {
      statements.push_back(declaration());
    }

    consume(RIGHT_BRACE,"Expect '}' after block.");
    return statements;
  }

  Token* Parser::consume(TokenType type, const char* message) {
    if (check(type)) return advance();

    throw lox_error::ParseError(peek(), message);
  }

  bool Parser::match(std::vector<TokenType> types) {
    for (const auto& type : types) {
      if (check(type)) {
        advance();
        return true;
      }
    }

    return false;
  }

  bool Parser::match(TokenType type) {
    if (check(type)) {
      advance();
      return true;
    }
    return false;
  }

  bool Parser::match(TokenType type, TokenType type2) {
    if (check(type) || check(type2)) {
      advance();
      return true;
    }
    return false;
  }

  bool Parser::check(lox_types::TokenType token_type) {
    if (isAtEnd()) return false;
    return peek()->type_ == token_type;
  }

  void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
      if (previous()->type_ == SEMICOLON) return;

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
  Token* Parser::advance() {
    if (!isAtEnd()) {
      current_++;
    }
    return previous();
  }

  bool Parser::isAtEnd() {
    return peek()->type_ == END_OF_FILE;
  }

  Token* Parser::peek() {
    return &tokens_.at(current_);
  }

  Token* Parser::previous() {
    return &tokens_.at(current_ - 1);
  }

