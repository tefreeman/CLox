#include "Lox.h"
#include "Parser.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Stmt.h"
#include "Resolver.h"

void Lox::Run(const std::string& content) {
   Scanner scanner(content);
   std::vector<Token> tokens = scanner.ScanTokens();
   
   Parser parser(tokens);
   std::vector<Stmt*> statements = parser.parse();



   if (lox_error::had_error) return;

   if (lox_error::had_runtime_error) {
     exit(70);
   }
   interpreter_ = new Interpreter();
   Resolver resolver =  Resolver(interpreter_);
   resolver.resolve(statements);


   if (lox_error::had_error) return;

   if (lox_error::had_runtime_error) {
     exit(70);
   }

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
