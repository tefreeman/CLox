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

    if (argc > 2) {
        std::cout << "Usage: lox [script]" << std::endl;
        std::exit(64);
    }
    else if (argc == 2) {
          if (strcmp(argv[1], "-test") == 0) {
          // do test
            std::cout << "Running test" << std::endl;

            lox_testing::run_test(lox_testing::test_str_1, lox_testing::expected_1);
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
