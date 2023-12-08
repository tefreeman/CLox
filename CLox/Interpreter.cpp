#include "Interpreter.h"
#include "LoxTypes.h"
#include"LoxError.h"
#include "objects\LoxCallable.h"
#include "objects\LoxFunction.h"
#include "EnvironmentManager.h"
#include "objects\LoxClass.h"
#include "objects\LoxInstance.h"
#include <any>
#include <regex>
#include "LoxConsole.h"
using namespace lox_types;

std::any Interpreter::evaluate(Expr* expr)
{
  return expr->accept(this);
}

bool Interpreter::IsTruthy(std::any val)
{
  if (val.type() == typeid(nullptr_t))
    return false;

  if (val.type() == typeid(bool))
    return std::any_cast<bool>(val);
  
  // everything else is true
  return true;
  }

  // TODO verify this works
bool Interpreter::IsEqual(std::any val, std::any val2)
{
  if (val.type() == val2.type()) {
    if (val.type() == typeid(double)) {
      return std::any_cast<double>(val) == std::any_cast<double>(val2);
    } 
    else if (val.type() == typeid(std::string)) {
      return std::any_cast<std::string>(val) == std::any_cast<std::string>(val2);
    }
    else if (val.type() == typeid(bool)) {
      return std::any_cast<bool>(val) == std::any_cast<bool>(val2);
    }
    else if (val.type() == typeid(nullptr_t)) {
      return true;
    }
    else if (val.type() == typeid(LoxClass*)) {
      return std::any_cast<LoxClass*>(val) == std::any_cast<LoxClass*>(val2);
    }
    else if (val.type() == typeid(LoxFunction*)) {
      return std::any_cast<LoxFunction*>(val) == std::any_cast<LoxFunction*>(val2);
    }
    else if (val.type() == typeid(LoxInstance*)) {
      return std::any_cast<LoxInstance*>(val) == std::any_cast<LoxInstance*>(val2);
    }
    else if (val.type() == typeid(LoxCallableClock*)) {
      return std::any_cast<LoxCallableClock*>(val) == std::any_cast<LoxCallableClock*>(val2);
    }
    else if (val.type() == typeid(LoxCallableConsoleHistory*)) {
      return std::any_cast<LoxCallableConsoleHistory*>(val) == std::any_cast<LoxCallableConsoleHistory*>(val2);
    }
  }
  return false;
}
void Interpreter::CheckNumberOperand(Token* op, std::any operand)
{
  if (operand.type() == typeid(double)) return;

  throw lox_error::RunTimeError(op, "Operand must be a number.");

}
void Interpreter::CheckNumberOperand(Token* op, std::any left, std::any right)
{
  if (left.type() == typeid(double) && right.type() == typeid(double)) return;

  throw lox_error::RunTimeError(op, "Operands must be numbers.");
}
std::string Interpreter::Stringify(std::any value)
{
  if (value.type() == typeid(double)) {
    return std::to_string(std::any_cast<double>(value));
  }
  else if (value.type() == typeid(nullptr_t)) {
    return "nil";
  }
  else if (value.type() == typeid(bool)) {
    return std::to_string(std::any_cast<bool>(value));
  }
  else if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  }
  else if (value.type() == typeid(LoxClass*)) {
    LoxClass* klass =  std::any_cast<LoxClass*>(value);
    return klass->toString();
  }
  else if (value.type() == typeid(LoxInstance*)) {
    LoxInstance* instance = std::any_cast<LoxInstance*>(value);
    return instance->toString();
  }
  else if (value.type() == typeid(LoxFunction*)) {
    LoxFunction* func = std::any_cast<LoxFunction*>(value);
    return func->ToString();
  }
  else if (value.type() == typeid(LoxCallableClock*)) {
    LoxCallableClock* func = std::any_cast<LoxCallableClock*>(value);
    return func->toString();
  }

  
  return "INVALID std::any value in Interpreter::stringify";
}
void Interpreter::execute(Stmt* stmt)
{
  stmt->accept(this);
}
std::any Interpreter::lookUpVariable(Token* name, Expr* expr)
{
  auto it = locals_.find(expr);
  if (it != locals_.end()) {
    
    return environment_->getAt(it->second, name->lexeme_);
  }
  else {
    return globals_->get(name);
  }
}
void Interpreter::executeBlock(std::vector<Stmt*> statements, Environment* env)
{
  EnvironmentManager envManager(environment_, env);

  // TODO check if this works (microsoft error handling)
   //environment_ = env;

    for (Stmt* statement : statements) {
      execute(statement);
    }

}
Interpreter::Interpreter()
{
  environment_ = globals_;
  globals_->define("clock", new LoxCallableClock());
  //globals_->define("console", new LoxCallableConsoleHistory());
}
void Interpreter::Interpret(std::vector<Stmt*> statements)
{
  try {
    for (Stmt* statement : statements) {
      execute(statement);
    }
  }
  catch (lox_error::RunTimeError& error) {
    error.display();
  }
}
void Interpreter::resolve(Expr* expr, int depth)
{
  locals_.insert_or_assign(expr, depth);
}
std::any Interpreter::visit(Literal* expr)
{
    return expr->value_;
}

std::any Interpreter::visit(Grouping* expr)
{
  return evaluate(expr->expression_);
}

std::any Interpreter::visit(Unary* expr)
{
  std::any right = evaluate(expr->right_);

  switch (expr->op_->type_) {
    case BANG:
      return !IsTruthy(right);
    case MINUS:
      CheckNumberOperand(expr->op_, right);
      return -(std::any_cast<double>(right));
    break;
  }
  return nullptr;
}

std::any Interpreter::visit(Binary* expr)
{
  std::any left = evaluate(expr->left_);
  std::any right = evaluate(expr->right_);

  switch (expr->op_->type_) {
    case MINUS:
      CheckNumberOperand(expr->op_, left, right);
      return std::any_cast<double>(left) - std::any_cast<double>(right);
    case SLASH:
      CheckNumberOperand(expr->op_, left, right);
      return std::any_cast<double>(left) / std::any_cast<double>(right);
    case STAR:
      CheckNumberOperand(expr->op_, left, right);
      return std::any_cast<double>(left) * std::any_cast<double>(right);

    case PLUS:
      if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return std::any_cast<double>(left) + std::any_cast<double>(right);
      }
      if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
      }

      throw  lox_error::RunTimeError(expr->op_, "Operands must be two numbers or two strings.");
      
      case GREATER:
        CheckNumberOperand(expr->op_, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
      case GREATER_EQUAL:
        CheckNumberOperand(expr->op_, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
      case LESS:
        CheckNumberOperand(expr->op_, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
      case LESS_EQUAL:
        CheckNumberOperand(expr->op_, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);

      case BANG_EQUAL:
        return !(IsEqual(left, right));
      case EQUAL_EQUAL:
        return IsEqual(left, right);

      break;
  }

  return nullptr;
}

std::any Interpreter::visit(Variable* expr)
{
  return lookUpVariable(expr->name_, expr);
}

std::any Interpreter::visit(Assign* expr)
{
  std::any value = evaluate(expr->value_);
  auto it = locals_.find(expr);
  if (it != locals_.end()) {
    environment_->assignAt(it->second, expr->name_, value);
  }
  else {
    globals_->assign(expr->name_, value);
  }
  return value;
}

std::any Interpreter::visit(Logical* expr)
{
  std::any left = evaluate(expr->left_);

  if (expr->op_->type_ == lox_types::OR) {
    if(IsTruthy(left)) return left;
  }
  else {
    if (!IsTruthy(left)) return left;
  }

  return evaluate(expr->right_);
}

std::any Interpreter::visit(CallExpr* expr)
{
  std::any callee = evaluate(expr->callee_);

  std::vector<std::any> arguments;

  for (Expr* argument : expr->arguments_) {
    arguments.push_back(evaluate(argument));
  }

  LoxCallable* callable = nullptr;

  // TODO implement this way for all any_cast
  // What if more types of LoxCallable?

  if (callee.type()  == typeid(LoxFunction*)) {
    callable = std::any_cast<LoxFunction*>(callee);
  }
  else if (callee.type() == typeid(LoxClass*)) {
    callable = std::any_cast<LoxClass*>(callee);
  }
  else if (callee.type() == typeid(LoxCallableClock*)) {
    callable = std::any_cast<LoxCallableClock*>(callee);
  }
  else if (callee.type() == typeid(LoxCallableConsoleHistory*)) {
    callable = std::any_cast<LoxCallableConsoleHistory*>(callee);
  }
  // TODO verify it works
  if (callable == nullptr) {
    throw  lox_error::RunTimeError(expr->paren_, "Can only call functions and classes.");
  }

  if (arguments.size() != callable->arity()) {
    std::string msg = "Expected " + std::to_string(callable->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".";
    throw lox_error::RunTimeError(expr->paren_, msg.c_str());
  }
  return callable->Call(this, arguments);
}

std::any Interpreter::visit(Get* expr)
{

  std::any object = evaluate(expr->object_);
  LoxInstance* tryLoxInstance = nullptr;

  if (object.type() == typeid(LoxInstance*)) {
    tryLoxInstance = std::any_cast<LoxInstance*>(object);
    }

  if (tryLoxInstance != nullptr) {
    return tryLoxInstance->get(expr->name_);
  }

  throw  lox_error::RunTimeError(expr->name_,
    "Only instances have properties.");
}

std::any Interpreter::visit(Set* expr)
{
  std::any object = evaluate(expr->object_);
  LoxInstance* tryLoxInstance = nullptr;
 
 if (object.type() == typeid(LoxInstance*)) {
  tryLoxInstance = std::any_cast<LoxInstance*>(object);
  }

  if (tryLoxInstance == nullptr) {
    throw  lox_error::RunTimeError(expr->name_,
      "Only instances have fields.");
  }

  std::any value = evaluate(expr->value_);

  tryLoxInstance->set(expr->name_, value);
  return value;
}

std::any Interpreter::visit(This* expr)
{
  return lookUpVariable(expr->keyword_, expr);
}

std::any Interpreter::visit(Super* expr)
{
  auto it = locals_.find(expr);
  int distance = it->second;

  std::any superclassAny = environment_->getAt(distance, "super");
  std::any objectany = environment_->getAt(distance - 1, "this");

  LoxClass* superclass = std::any_cast<LoxClass*>(environment_->getAt(distance, "super"));
  LoxInstance* object = std::any_cast<LoxInstance*>(environment_->getAt(distance-1, "this"));

  LoxFunction* method = superclass->findMethod(expr->method_->lexeme_);
 
  if (method == nullptr) {
    std::string errMsg = "Undefined property '" + expr->method_->lexeme_ + "'.";
    throw lox_error::RunTimeError(expr->method_, errMsg.c_str());
  }

 return method->bind(object);
}

void Interpreter::visit(Expression* exprStmt)
{
  evaluate(exprStmt->expression_);
  return;
}

void Interpreter::visit(Print* stmt)
{
  std::any value = evaluate(stmt->expression_);
  std::string str = Stringify(value);

  // Added new line detection
  std::string const result = std::regex_replace(str, std::regex("\\\\n"), "\n") + "\n";
 
  lox_console::print(result);
}

void Interpreter::visit(Var* stmt)
{
  std::any value = nullptr;

  if (stmt->initalizer_ != nullptr) {
    value = evaluate(stmt->initalizer_);
  }

  environment_->define(stmt->name_->lexeme_, value);
  return;
}

void Interpreter::visit(Block* stmt)
{
  executeBlock(stmt->statements_, new Environment(environment_));
  return;
}

void Interpreter::visit(If* stmt)
{
  if (IsTruthy(evaluate(stmt->condition_))) {
    execute(stmt->thenBranch_);
  }
  else if (stmt->elseBranch_ != nullptr) {
    execute(stmt->elseBranch_);
  }
  return;
}

void Interpreter::visit(While* stmt)
{
  while (IsTruthy(evaluate(stmt->condition_))) {
    execute(stmt->body_);
  }
  return;
}

void Interpreter::visit(Function* stmt)
{
  LoxFunction* function = new LoxFunction(stmt, environment_, false);
  environment_->define(stmt->name_->lexeme_, function);
  return;
}

void Interpreter::visit(Return* stmt)
{
  std::any value = nullptr;

  if (stmt->value_ != nullptr) {
    value = evaluate(stmt->value_);
  }

  throw lox_error::ReturnException(value, "ReturnException");
}

void Interpreter::visit(Class* stmt)
{
  std::any superclass;

  if (stmt->superclass_ != nullptr) {
    superclass = evaluate(stmt->superclass_);


    if (superclass.type() != typeid(LoxClass*)) {
      throw lox_error::RunTimeError(stmt->superclass_->name_, "Superclass must be a class.");
    }


  }

  environment_->define(stmt->name_->lexeme_, nullptr);
  
  if (stmt->superclass_ != nullptr) {
    environment_ = new Environment(environment_);
    environment_->define("super", superclass);
  }

  std::unordered_map<std::string, LoxFunction*> methods;
  
  for (Function* method : stmt->methods_) {
    LoxFunction* function = new LoxFunction(method, environment_, method->name_->lexeme_== "init");
    methods.insert_or_assign(method->name_->lexeme_, function);
  }
  
  LoxClass* superClassCasted = nullptr;

  if (superclass.has_value())
    superClassCasted = std::any_cast<LoxClass*>(superclass);

  LoxClass* klass = new LoxClass(stmt->name_->lexeme_, superClassCasted, methods);

  // If empty then stmt->superclass_ is nullptr
  if (superclass.has_value()) {
    environment_ = environment_->enclosing_;
  }
  environment_->assign(stmt->name_, klass);
  return;
}

