#pragma once
#include "objects/Expr.h"
#include <any>
#include <ostream>
#include <sstream>


class AstPrinter : public Expr::Visitor<std::any> {
private:

  std::string parenthesize(std::string name, Expr* expr) {
    std::ostringstream builder;

    builder << "(" << name;
    builder << " ";
    std::string str = std::any_cast<std::string>(expr->accept(this)); // Assuming accept method returns std::string)
    builder << str; // Assuming accept method returns std::string
    builder << ")";

    return builder.str();
  }

  std::string parenthesize(std::string name, Expr* expr, Expr* expr2) {
    std::ostringstream builder;

    builder << "(" << name;

    builder << " ";
    std::string str = std::any_cast<std::string>(expr->accept(this));; // Assuming accept method returns std::string)
    builder << str; // Assuming accept method returns std::string

    builder << " ";
    std::string str2 = std::any_cast<std::string>(expr2->accept(this)); // Assuming accept method returns std::string)
    builder << str2; // Assuming accept method returns std::string

    builder << ")";

    return builder.str();
  }

  std::string parenthesize(const std::string name, const std::vector<Expr*>& exprs) {
    std::ostringstream builder;

    builder << "(" << name;
    for (const auto& expr : exprs) {
      builder << " ";
      std::string str = std::any_cast<std::string>(expr->accept(this)); // Assuming accept method returns std::string)
      builder << str; // Assuming accept method returns std::string
    }
    builder << ")";

    return builder.str();
  }


  std::string LiteralToString(std::any value) {
    if (value.type() == typeid(std::string))
      return std::any_cast<std::string> (value);

    if (value.type() == typeid(float))
      return std::to_string(std::any_cast<float> (value));

    if (value.type() == typeid(int))
      return std::to_string(std::any_cast<int> (value));

    if (value.type() == typeid(double))
      return std::to_string(std::any_cast<double> (value));

    if (value.type() == typeid(bool))
      return std::to_string(std::any_cast<bool> (value));

    if (value.type() == typeid(nullptr)) {
      return "nil";
      }
    return "LITERAL TO STRING TYPE NOT SUPPORTED";
  }

  public:
  std::string print(Expr* expr) {
      return std::any_cast<std::string>(expr->accept(this));
  }

  std::any visit(Assign* expr) {
    return "";
  }
  std::any visit(Binary* expr) {
    return parenthesize(expr->op_->lexeme_, expr->left_, expr->right_);
  }
  std::any visit(Grouping* expr) {
    return parenthesize("group", expr->expression_);
  }
   std::any visit(Unary* expr) {
    return parenthesize(expr->op_->lexeme_, expr->right_);
  }
   std::any visit(Literal* expr) {
     if (expr->value_.has_value() == false) {
      return "nil";
    }
    return LiteralToString(expr->value_);
  }
};
