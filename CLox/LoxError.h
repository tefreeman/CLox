#pragma once
#include <string>
#include <iostream>

// Error system moved to seperate file to avoid circular dependencies
namespace lox_error
{
  bool had_error = false;

  void Report(int line, std::string where, std::string& message)
  {
    std::cout << "[line " << line << "] ";
    std::cout << "Error " << where << ": ";
    std::cout << message << std::endl;
    had_error = true;
  }

  void Error(int line, std::string message)
  {
    Report(line, "", message);
  }
};

