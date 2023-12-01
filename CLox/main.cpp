  #include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "Lox.h"
#include "Tests.h"
#include "AstPrinter.h"
#include <filesystem>

int main(int argc, char* argv[]) {
  
    Lox lox = Lox();

    if (argc > 2) {
        std::cout << "Usage: lox [script]" << std::endl;
        std::exit(64);
    }
    else if (argc == 2) {
          if (strcmp(argv[1], "-test") == 0) {
          // do test
            std::filesystem::path cwd = std::filesystem::current_path();
            std::string path = cwd.string() + "/tests";

            std::filesystem::recursive_directory_iterator it(path);

            for (auto& p : it) {
              if (p.path().extension() == ".txt") {
                std::cout << "Running test: " << p.path().filename() << std::endl;
                lox.RunFile(p.path().string());
              }
            }
            }
          else if (strcmp(argv[1], "-debug") == 0) {
            lox.debug_mode_ = true;
            lox.RunPrompt();
          } else {
          lox.RunFile(argv[1]);
        }
    }
    else {
        lox.RunPrompt();
    }
    
    return 0;
}
