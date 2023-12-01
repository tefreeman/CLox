#include "LoxInstance.h"
#include "LoxClass.h"
#include "LoxError.h"
#include "LoxFunction.h"

LoxInstance::LoxInstance(LoxClass* klass) {
      klass_ = klass;
    };

    std::string LoxInstance::toString() {
      return klass_->name_ + " instance";
    }
    std::any LoxInstance::get(Token* name)
    {
      auto it = fields_.find(name->lexeme_);
      if (it != fields_.end()) {
         return it->second;
      }
      LoxFunction* method = klass_->findMethod(name->lexeme_);
      if (method != nullptr) return method->bind(this);

      std::string msg = "Undefined property '" + name->lexeme_ + "'.";
      throw  lox_error::RunTimeError(name, msg.c_str());
    }

    void LoxInstance::set(Token* name, std::any value)
    {
      fields_.insert_or_assign(name->lexeme_, value);
    }


  