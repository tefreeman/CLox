#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include "Scanner.h"
class Lox
{
private:
public:
	bool debug_mode_ = false;
	void RunFile(const std::string& path);
	void Run(const std::string& content);
	void RunPrompt();
};

