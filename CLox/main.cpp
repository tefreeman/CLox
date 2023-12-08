#include "AstPrinter.h"
#include "Lox.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define TEST_FOLDER "\\tests"
#define FILE_EXTENSION ".lox"
#define REDIRECT_TO_OUT_FILE false



int main(int argc, char* argv[]) {

  if (REDIRECT_TO_OUT_FILE)
  {
    std::locale::global(std::locale("en_US.UTF-8"));
    std::ofstream out("out.txt");
    auto* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
  }

  Lox lox = Lox();

  if (argc > 2) {
    std::cout << "Usage: lox [script]" << std::endl;
    std::exit(64);
  }
  else if (argc == 2) {
    if (strcmp(argv[1], "-test") == 0) {
      // do test
      std::filesystem::path cwd = std::filesystem::current_path();
      std::string path = cwd.string() + TEST_FOLDER;

      std::filesystem::recursive_directory_iterator it(path);

      for (auto& p : it) {
        if (p.path().extension() == FILE_EXTENSION) {
          lox.TestRunFile(p.path().string(), REDIRECT_TO_OUT_FILE);
        }
      }
    }
    else {
      lox.RunFile(argv[1]);
    }
  }
  else {
    lox.RunPrompt();
  }

  return 0;
}
