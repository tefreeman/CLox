#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "LoxTypes.h"

using namespace lox_types;

class Scanner
{
private:
  std::string source_;
  std::vector<Token> tokens_;
  
  int start_ = 0;
  int current_ = 0;
  int line_ = 1;

  void ScanToken();
  bool IsAtEnd();
  char Advance();
  bool Match(char expected);
  char PeekNext();
  char Peek();
  void AddToken(TokenType type);
  void AddToken(TokenType type, Literal literal);
  bool IsDigit(char c);
  void NumberHandler();

  void Identifier();
  bool IsAlpha(char c);
  bool IsAlphaNumeric(char c);

  void StringHandler();

public:
  Scanner(std::string source) {
    source_ = source;
  }

  std::vector<Token> ScanTokens();

};

