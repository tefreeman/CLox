#pragma once
#include "LoxTypes.h"
#include <string>
#include <variant>
#include <any>

typedef std::any LiteralType;
using namespace lox_types;

class Token
{
public:
	TokenType type_;
	std::string lexeme_;
	LiteralType literal_;
	int line_;

	Token(TokenType type, std::string lexeme, LiteralType literal, int line) {
		type_ = type;
		lexeme_ = lexeme;
		literal_ = literal;
		line_ = line;
	}

	std::string ToString() {
		return "Type: " + TypeToString() + ", Lexeme: " + lexeme_ + ", Literal: " + LiteralToString() + " Line: " + std::to_string(line_);
	}

  // TODO add literal when it is implemented
  bool operator==(Token& rhs) {
    return ((type_ == rhs.type_) && (lexeme_ == rhs.lexeme_)  && (line_ == rhs.line_)) && (LiteralToString() == rhs.LiteralToString());
  }
private:
  
  // uses a enum, string mapping in LoxTypes.h
  std::string TypeToString() {
    return tokenTypeToString.at(type_);
  }

  std::string LiteralToString() {
    //TODO FIX
    // Return an empty string as a fallback (this should never be reached)
    return "Null";
  }
};

