#include "LoxClass.h"
#include "LoxInstance.h"


LoxClass::LoxClass(std::string& name, LoxClass* superclass, std::unordered_map<std::string, LoxFunction*> methods)
{
name_ = name;
superclass_ = superclass;
methods_ = methods;
}

std::string LoxClass::toString() {
  return name_;
}

LoxFunction* LoxClass::findMethod(std::string name)
{
  auto it = methods_.find(name);
  if (it != methods_.end()) {
    return it->second;
  }
  if (superclass_ != nullptr) {
    return superclass_->findMethod(name);
  }

  return nullptr;
}

std::any LoxClass::Call(Interpreter* interpreter, std::vector<std::any> arguments) {
  LoxInstance* instance = new LoxInstance(this);
  LoxFunction* initializer = findMethod("init");
  if (initializer != nullptr) {
    initializer->bind(instance)->Call(interpreter, arguments);
  }

  return instance;
}
int LoxClass::arity() {
  LoxFunction* initializer = findMethod("init");
  if (initializer == nullptr) return 0;
  return initializer->arity();
}