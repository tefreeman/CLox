#pragma once
#include <string>
#include "Scanner.h"
#include "LoxTypes.h"

using namespace lox_types;

namespace lox_testing {

  std::string test_str_1 = "var count = 10; \nname = \"Trevor\";";
  std::vector<Token> expected_1 = {
  Token(VAR, "var", nullptr, 1), 
  Token(IDENTIFIER, "count", nullptr, 1),
  Token(EQUAL, "=", nullptr, 1),
  Token(NUMBER, "10", nullptr, 1),
  Token(SEMICOLON, ";", nullptr, 1),
  Token(IDENTIFIER, "name", nullptr, 2),
  Token(EQUAL, "=", nullptr, 2),
  Token(STRING, "\"Trevor\"", "Trevor", 2),
  Token(SEMICOLON, ";", nullptr, 2),
  Token(END_OF_FILE, "", nullptr, 2)
  };


  std::string test_str_2 = "var multi_line_test = \"one line\ntwo line\nthree line\";";
  std::vector<Token> expected_2 = {
    Token(VAR, "var", nullptr, 1),
    Token(IDENTIFIER, "multi_line_test", nullptr, 1),
    Token(EQUAL, "=", nullptr, 1),
    Token(STRING, "one line\ntwo line\nthree line", nullptr, 3),
    Token(SEMICOLON, ";", nullptr, 3),
    Token(END_OF_FILE, "", nullptr, 3)

};

  bool run_test(std::string test_str, std::vector<Token> expected_tokens) {
    Scanner scanner(test_str);
    std::vector<Token> tokens = scanner.ScanTokens();
    int i = 0;
    bool has_error = false;

    for (Token token : tokens) {
      if (token == expected_tokens.at(i)) {
      }
      else {
       has_error = true;
        std::cout << "Test Failure" << std::endl;
        std::cout << "token:  " + token.ToString() << std::endl;
        std::cout << "expected: " + expected_tokens.at(i).ToString() << std::endl;
      }
      i++;
    }

    return has_error;
  }
}