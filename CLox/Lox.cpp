#include "Lox.h"
#include "Parser.h"
#include "objects\Expr.h"
#include "AstPrinter.h"
#include "objects\Stmt.h"
#include "Resolver.h"
#include "LoxTest.h"
#include "LoxConsole.h"
#include <vector>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "Scanner.h"
#include "Parser.h"

void Lox::Run(const std::string& content) {
   Scanner scanner(content);

   std::vector<Token> tokens = scanner.ScanTokens();
   
   Parser parser(tokens);
   std::vector<Stmt*> statements = parser.parse();


     if (lox_error::had_error) return;


     interpreter_ = new Interpreter();

     Resolver resolver =  Resolver(interpreter_);
     resolver.resolve(statements);

     if (lox_error::had_runtime_error) return;
     if (lox_error::had_error) return;



     interpreter_->Interpret(statements);
 

   /*
   AstPrinter* ast = new AstPrinter();
   std::cout << ast->print(expression);
   */

   if (debug_mode_) {
     for (Token token : tokens) {
       std::cout << token.ToString() << std::endl;
     }
   }

}


void Lox::RunFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file.");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);

    if (file.read(buffer.data(), size)) {
        std::string content(buffer.begin(), buffer.end());
        Run(content);
    }
    else {
        throw std::runtime_error("Unable to read file.");
    }
}

void Lox::TestRunFile(const std::string& path, bool redirectOutput) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file.is_open()) {
    throw std::runtime_error("Unable to open file.");
  }

  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);

  if (file.read(buffer.data(), size)) {
    std::string content(buffer.begin(), buffer.end());
    std::string filename = path.substr(path.find_last_of("/\\")+ 1);
    LoxTest test = LoxTest(path , content, redirectOutput);
    
    test.printStart();

    Run(content);


    test.validate();
    test.printResults();

    lox_console::clear();

    // reset errors? may need to change into a test mode
    lox_error::had_error = false;
    lox_error::had_runtime_error = false;
  }
  else {
    throw std::runtime_error("Unable to read file.");
  }
}

void Lox::RunPrompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (std::cin.eof()) {
            break;
        }
        this->Run(line);
    }
}
