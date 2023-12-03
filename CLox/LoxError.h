#pragma once
#include <string>
#include <iostream>
#include <exception>
#include "Token.h"
#include "LoxTypes.h"
#include "LoxConsole.h"


// Error system moved to seperate file to avoid circular dependencies
namespace lox_error
{
  inline bool had_error = false;
  inline bool had_runtime_error = false;

  inline void Report(int line, std::string where, std::string& message)
  {
    std::string msg =  "[line " + std::to_string(line) + "] " + "Error" + where + ": " + message +"\n";
    lox_console::print(msg); 
    had_error = true;
  }

  inline void Report(int line, std::string where, const char* message)
  {
    std::string msg = "[line " + std::to_string(line) + "] " + "Error" + where + ": " + message +"\n";
    lox_console::print(msg);
    had_error = true;
  }

  inline void ReportRuntime(int line, std::string where, const char* message) {
    std::string msg = std::string("error at '") + where + "': " + message + std::string("[line " + std::to_string(line) + "] ") + "\n";
    lox_console::print(msg);
    had_runtime_error = true;
  }

  inline void ReportRuntime(int line, std::string where, std::string message) {
    std::string msg = "error at '" + where + "': " + message + std::string("[line " + std::to_string(line) + "] ") + "\n";
    lox_console::print(msg);
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
    std::string message_;

  public:
    inline RunTimeError(Token* token, const char* message) : std::exception(message) {
      token_ = token;
      message_ = std::string(message);
    };

    inline void display() {
      ReportRuntime(token_->line_, token_->lexeme_, message_);
    }

    const char* what() const throw () {
      return "RunTimeError";
    }


  };

  class ReturnException : public std::exception {
  
  public:
    std::any value_;
    inline ReturnException(std::any& value, const char* message) : std::exception(message) {
    value_ = value;
    }


    const char* what() const throw () {
      return "ReturnException";
    }
  };

};

