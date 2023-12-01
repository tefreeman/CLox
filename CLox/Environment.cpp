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
  auto search = values_.find(name->lexeme_);
  if (search != values_.end()) {
    search->second = value;
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
  auto search = values_.find(name->lexeme_);
  if (search != values_.end()) {
    return search->second;
  }

  if (enclosing_ != nullptr) return enclosing_->get(name);

  std::string errorMsg = "Undefined variable '" + name->lexeme_ + "'.";
  throw lox_error::RunTimeError(name, errorMsg.c_str());
}

std::any Environment::getAt(int distance, std::string name)
{
  return ancestor(distance)->values_.find(name)->second;
}

Environment* Environment::ancestor(int distance)
{
  Environment* environment = this;
  for (int i = 0; i < distance; i++) {
    environment = environment->enclosing_;
  }

  return environment;
}

void Environment::assignAt(int distance, Token* name, std::any value)
{
  ancestor(distance)->values_.insert_or_assign(name->lexeme_, value);
}
