#pragma once
#include <string>
#include <iostream>
#include <exception>
#include "Token.h"
#include "LoxTypes.h"



// Error system moved to seperate file to avoid circular dependencies
namespace lox_error
{
  inline bool had_error = false;
  inline bool had_runtime_error = false;

  inline void Report(int line, std::string where, std::string& message)
  {
    std::cout << "[line " << line << "] ";
    std::cout << "Error " << where << ": ";
    std::cout << message << std::endl;
    had_error = true;
  }

  inline void Report(int line, std::string where, const char* message)
  {
    std::cout << "[line " << line << "] ";
    std::cout << "Error " << where << ": ";
    std::cout << message << std::endl;
    had_error = true;
  }

  inline void ReportRuntime(int line, const char* message) {
    std::cout << message <<  std::endl;
    std::cout << "[line " << line << "] ";
    had_runtime_error = true;
  }
  // TODO get rid of this func?
  inline void Error(int line, std::string message)
  {
    Report(line, "", message);
  }


  inline void Error(Token* token, const char* message)
  {
    if (token->type_ == lox_types::END_OF_FILE) {
      Report(token->line_, " at end", message);
    }
    else {
      Report(token->line_, " at '" + token->lexeme_ + "'", message);
    }
  }

  class ParseError : public std::exception {
    private:
      Token* token_;
      const char* message_;

    public:
      inline ParseError(Token* token, const char* message): std::exception(message)  {
        token_ = token;
        message_ = message;
      };

      inline void display() {
        Error(token_, message_);
      }

      const char* what() const throw () {
        return "ParseError";
      }


  };
  
  class RunTimeError : public std::exception {
  private:
    Token* token_;
    const char* message_;

  public:
    inline RunTimeError(Token* token, const char* message) : std::exception(message) {
      token_ = token;
      message_ = message;
    };

    inline void display() {
      ReportRuntime(token_->line_, message_);
    }

    const char* what() const throw () {
      return "RunTimeError";
    }


  };

};

