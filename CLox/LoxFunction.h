#pragma once
#include "LoxCallable.h"

class LoxFunction : public LoxCallable {
  private:
  public:
    Function* declaration_;

    LoxFunction(Function* declaration) {
      declaration_ = declaration;
    }

    std::any Call(Interpreter* interpreter, std::vector<std::any> arguments) {
      Environment* environment = new Environment(interpreter->globals_);
      
      for (int i = 0; i < declaration_->params_.size(); i++) {
        environment->define(declaration_->params_.at(i)->lexeme_,
          arguments.at(i));
      }

      interpreter->executeBlock(declaration_->body_, environment);
      return nullptr;
     }

    int arity() {
      return declaration_->params_.size();
    }

     std::string ToString() {
      return "<fn " + declaration_->name_->lexeme_ + ">";
    }

};