#include "Lox.h"

void Lox::Run(const std::string& content) {
   Scanner scanner(content);
   std::vector<Token> tokens = scanner.ScanTokens();


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