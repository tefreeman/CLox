#include "LoxFunction.h"
#include "..\Environment.h"
#include "..\LoxError.h"
#include <exception>
#include <string>
#include "LoxInstance.h"
LoxFunction::LoxFunction(Function* declaration, Environment* closure, bool isInitializer) {
  closure_ = closure;
  declaration_ = declaration;
  isInitializer_ = isInitializer;
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
    if (isInitializer_) return closure_->getAt(0, "this");
    return returnValue.value_;
  }
  if (isInitializer_) return closure_->getAt(0, "this");
  return nullptr;
}

int LoxFunction::arity() {
  return declaration_->params_.size();
}

std::string LoxFunction::ToString() {
  return "<fn " + declaration_->name_->lexeme_ + ">";
}

LoxFunction* LoxFunction::bind(LoxInstance* instance)
{
  Environment* environment = new Environment(closure_);
  environment->define("this", instance);
  return new LoxFunction(declaration_, environment, isInitializer_);
}
