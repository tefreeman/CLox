#include "Resolver.h"
#include <unordered_map>
#include <stack>
#include "LoxError.h"
#include <deque>
Resolver::Resolver(Interpreter* interpreter)
{
  interpreter_ = interpreter;
  currentFunction_ = FunctionType::NONE;
  currentClass_ = ClassType::NONE;
}

void Resolver::visit(Variable* expr)
{
  if (!scopes.empty()) {
    std::unordered_map<std::string, bool>::const_iterator it = scopes.back().find(expr->name_->lexeme_);
    
    if (it != scopes.back().end()) {
      if (it->second == false) {
      throw lox_error::RunTimeError(expr->name_,
        "Can't read local variable in its own initializer.");
        }
    }
  }

  resolveLocal(expr, expr->name_);
  return;
}

void Resolver::visit(Assign* expr)
{
  resolve(expr->value_);
  resolveLocal(expr, expr->name_);
  return;
}

void Resolver::visit(Binary* expr)
{
  resolve(expr->left_);
  resolve(expr->right_);
  return;
}

void Resolver::visit(CallExpr* expr)
{
  resolve(expr->callee_);

  for (Expr* argument : expr->arguments_) {
    resolve(argument);
  }

  return;
}

void Resolver::visit(Grouping* expr)
{
  resolve(expr->expression_);
  return;
}

void Resolver::visit(Literal* expr)
{
  return;
}

void Resolver::visit(Logical* expr)
{
  resolve(expr->left_);
  resolve(expr->right_);
  return;
}

void Resolver::visit(Unary* expr)
{
  resolve(expr->right_);
  return;
}


void Resolver::resolve(std::vector<Stmt*> statements)
{
  for (Stmt* statement : statements) {
    try {
      resolve(statement);
    }
    catch (lox_error::RunTimeError& e) {
      e.display();
    }
  }
}

void Resolver::resolve(Stmt* stmt)
{
  stmt->accept(this);
}

void Resolver::resolve(Expr* expr)
{
  expr->accept(this);
}

void Resolver::beginScope()
{
  scopes.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::endScope()
{
  scopes.pop_back();
}

void Resolver::declare(Token* name)
{
  if (scopes.empty()) return;
  if (auto search = scopes.back().find(name->lexeme_); search != scopes.back().end()) {
    throw lox_error::RunTimeError(name,
      "Already a variable with this name in this scope.");
  }
  scopes.back().insert_or_assign(name->lexeme_, false);
}

void Resolver::define(Token* name)
{
  if (scopes.empty()) return;
  scopes.back().insert_or_assign(name->lexeme_, true);
}

void Resolver::resolveLocal(Expr* expr, Token* name)
{
  int i = scopes.size() - 1;
  for (i; i >= 0; i--) {
    if (scopes.at(i).find(name->lexeme_) != scopes.at(i).end()) {
      interpreter_->resolve(expr, scopes.size() - 1 - i);
      return;
    }
  }
}

void Resolver::resolveFunction(Function* func, FunctionType type)
{
  FunctionType enclosingFunction = currentFunction_;
  currentFunction_ = type;

  beginScope();
  for (Token* param : func->params_) {
    declare(param);
    define(param);
  }
  resolve(func->body_);
  endScope();

  currentFunction_ = enclosingFunction;
}



void Resolver::visit(Get* expr)
{
  resolve(expr->object_);
  return;
}

void Resolver::visit(Block* stmt)
{
  beginScope();
  resolve(stmt->statements_);
  endScope();
  return;
}

void Resolver::visit(Set* expr)
{
  resolve(expr->value_);
  resolve(expr->object_);
  return;
}
void Resolver::visit(Super* expr) {
  if (currentClass_ == ClassType::NONE) {
    throw lox_error::RunTimeError(expr->keyword_,
      "Can't use 'super' outside of a class.");
  }
  else if (currentClass_ != ClassType::SUBCLASS) {
    throw lox_error::RunTimeError(expr->keyword_,
      "Can't use 'super' in a class with no superclass.");
  }
  resolveLocal(expr, expr->keyword_);
  return;
}

void Resolver::visit(Var* stmt)
{
  declare(stmt->name_);
  if (stmt->initalizer_ != nullptr) {
    resolve(stmt->initalizer_);
  }
  define(stmt->name_);
  return;
}

void Resolver::visit(Function* stmt)
{
  declare(stmt->name_);
  define(stmt->name_);

  resolveFunction(stmt, FunctionType::FUNCTION);
  return;
}

void Resolver::visit(This* expr)
{
  if (currentClass_ == ClassType::NONE) {
    throw lox_error::RunTimeError(expr->keyword_,
      "Can't use 'this' outside of a class.");
    return;
  }
  resolveLocal(expr, expr->keyword_);
  return;
}

void Resolver::visit(Expression* stmt)
{
  resolve(stmt->expression_);
  return;
}

void Resolver::visit(If* stmt)
{
  resolve(stmt->condition_);
  resolve(stmt->thenBranch_);
  if (stmt->elseBranch_ != nullptr) resolve(stmt->elseBranch_);
  return;
}

void Resolver::visit(Print* stmt)
{
  resolve(stmt->expression_);
  return;
}

void Resolver::visit(Return* stmt)
{
  if(currentFunction_ == FunctionType::NONE) {
    throw lox_error::RunTimeError(stmt->keyword_, "Can't return from top-level code.");
  }
  if (stmt->value_ != nullptr) {
    if (currentFunction_ == FunctionType::INITIALIZER) {
      throw lox_error::RunTimeError(stmt->keyword_,
        "Can't return a value from an initializer.");
    }
    resolve(stmt->value_);
  }

  return;
}

void Resolver::visit(While* stmt)
{
  resolve(stmt->condition_);
  resolve(stmt->body_);
  return;
}

void Resolver::visit(Class* stmt)
{
  ClassType enclosingClass = currentClass_;
  currentClass_ = ClassType::CLASS;

  declare(stmt->name_);
  define(stmt->name_);

  if (stmt->superclass_ != nullptr &&
    (stmt->name_->lexeme_ == stmt->superclass_->name_->lexeme_)) {
    throw lox_error::RunTimeError(stmt->superclass_->name_,
      "A class can't inherit from itself.");
  }


  if (stmt->superclass_ != nullptr) {
    currentClass_ = ClassType::SUBCLASS;
    resolve(stmt->superclass_);
  }

  if (stmt->superclass_ != nullptr) {
    beginScope();
    scopes.back().insert_or_assign("super", true);
  }

  beginScope();

  scopes.back().insert_or_assign("this", true);

  for (Function* method : stmt->methods_) {
    FunctionType declaration = FunctionType::METHOD;

    if (method->name_->lexeme_ == ("init")) {
      declaration = FunctionType::INITIALIZER;
    }
    resolveFunction(method, declaration);
  }
  endScope();

  if (stmt->superclass_ != nullptr) endScope();
  currentClass_ = enclosingClass;
  return;
}

