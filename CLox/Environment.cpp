#include "Environment.h"
#include "LoxError.h"

Environment::Environment()
{
  enclosing_ = nullptr;
}
Environment::Environment(Environment* enclosing)
{
  enclosing_ = enclosing;
}
void Environment::define(std::string key, std::any value)
{
  //TODO is this okay??
  values_.insert_or_assign(key, value);
}

void Environment::assign(Token* name, std::any value)
{
  if (auto search = values_.find(name->lexeme_); search != values_.end()) {
    values_[name->lexeme_] = value;
    return;
  }
  if (enclosing_ != nullptr) {
    enclosing_->assign(name, value);
    return;
  }
  std::string errorMsg = "Undefined variable '" + name->lexeme_ + "'.";
  throw lox_error::RunTimeError(name, errorMsg.c_str());
}

std::any Environment::get(Token* name)
{
  if (auto search = values_.find(name->lexeme_); search != values_.end()) {
    return search->second;
  }

  if (enclosing_ != nullptr) return enclosing_->get(name);

  std::string errorMsg = "Undefined variable '" + name->lexeme_ + "'.";
  throw lox_error::RunTimeError(name, errorMsg.c_str());
}