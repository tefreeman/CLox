#include "Interpreter.h"
#include "LoxTypes.h"
#include"LoxError.h"
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

  throw lox_error::RunTimeError(op, "Operands must be a number.");
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
  
  return "INVALID std::any value in Interpreter::stringify";
}
void Interpreter::execute(Stmt* stmt)
{
  stmt->accept(this);
}
Interpreter::Interpreter()
{

}
void Interpreter::Interpret(std::vector<Stmt*> statements)
{
  try {
    for (Stmt* statement : statements) {
      execute(statement);
    }
  }
  catch (lox_error::RunTimeError error) {
    error.display();
  }
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
      return std::any_cast<double>(right);
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
      return std::any_cast<double>(left) - std::any_cast<double>(right);
    case SLASH:
      return std::any_cast<double>(left) / std::any_cast<double>(right);
    case STAR:
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
        return std::any_cast<double>(left) > std::any_cast<double>(right);
      case GREATER_EQUAL:
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
      case LESS:
        return std::any_cast<double>(left) < std::any_cast<double>(right);
      case LESS_EQUAL:
        return std::any_cast<double>(left) <= std::any_cast<double>(right);

      case BANG_EQUAL:
        return !(IsEqual(left, right));
      case EQUAL_EQUAL:
        return IsEqual(left, right);

      break;
  }

  return nullptr;
}

void Interpreter::visit(Expression* exprStmt)
{
  evaluate(exprStmt->expression_);
  return;
}

void Interpreter::visit(Print* stmt)
{
  std::any value = evaluate(stmt->expression_);
  std::cout << Stringify(value);
  return;
}

