#pragma once
#include "LoxTypes.h"
#include <string>
#include <variant>

typedef std::variant<double, std::string, bool, nullptr_t> Literal;
using namespace lox_types;

class Token
{
public:
	TokenType type_;
	std::string lexeme_;
	Literal literal_;
	int line_;

	Token(TokenType type, std::string lexeme, Literal literal, int line) {
		type_ = type;
		lexeme_ = lexeme;
		literal_ = literal;
		line_ = line;
	}

	std::string ToString() {
		return "Type: " + TypeToString() + " Lexeme: " + lexeme_ + " Literal: " + LiteralToString() + " Line: " + std::to_string(line_);
	}

  // TODO add literal when it is implemented
  bool operator==(const Token& rhs) {
    return ((type_ == rhs.type_) && (lexeme_ == rhs.lexeme_)  && (line_ == rhs.line_));
  }
private:
  
  // uses a enum, string mapping in LoxTypes.h
  std::string TypeToString() {
    return tokenTypeToString.at(type_);
  }

  std::string LiteralToString() {
    std::visit([](const auto& val) {
      if constexpr (std::is_same_v<decltype(val), double>) {
        // If it's a double, convert to string
        return std::to_string(val);
      }
      else if constexpr (std::is_same_v<decltype(val), std::string>) {
        // If it's a string, no conversion needed
        return val;
      }
      else if constexpr (std::is_same_v<decltype(val), bool>) {
        // If it's a boolean, convert to "true" or "false"
        return val ? "true" : "false";
      }
      }, literal_);

    // Return an empty string as a fallback (this should never be reached)
    return "Null";
  }
};

