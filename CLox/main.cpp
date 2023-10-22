#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "Lox.h"
#include "Tests.h"


int main(int argc, char* argv[]) {
    Lox lox = Lox();
    lox_testing::run_test(lox_testing::test_str_1, lox_testing::expected_1);

    if (argc > 2) {
        std::cout << "Usage: lox [script]" << std::endl;
        std::exit(64);
    }
    else if (argc == 2) {
          if (argv[1] == "-test") {
          // do test
            std::cout << "testing...";
            }
          else if (argv[1] == "-debug") {
            
          } else {
          lox.RunFile(argv[1]);
        }
    }
    else {
        lox.debug_mode_ = true;
        lox.RunPrompt();
    }
    return 0;
}
