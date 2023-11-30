#include "LoxFunction.h"
#include "Environment.h"
#include "LoxError.h"
#include <exception>
#include <string>

LoxFunction::LoxFunction(Function* declaration, Environment closure) : declaration_(declaration) {
  closure_ = closure;
}

std::any LoxFunction::Call(Interpreter* interpreter, std::vector<std::any> arguments) {
  Environment* environment = new Environment(closure_);

  for (int i = 0; i < declaration_->params_.size(); i++) {
    environment->define(declaration_->params_.at(i)->lexeme_, arguments.at(i));
  }

  try {
    interpreter->executeBlock(declaration_->body_, environment);
  }
  catch (lox_error::ReturnException& returnValue) {
    return returnValue.value_;
  }

  return nullptr;
}

int LoxFunction::arity() {
  return declaration_->params_.size();
}

std::string LoxFunction::ToString() {
  return "<fn " + declaration_->name_->lexeme_ + ">";
}
