#pragma once
#include <iostream>
#include <string>
#include "Interpreter.h"

class Lox
{
private:
	Interpreter* interpreter_ = nullptr;
public:
	bool debug_mode_ = false;
	void RunFile(const std::string& path);
	void TestRunFile(const std::string& path, bool redirectOutput);
	void Run(const std::string& content);
	void RunPrompt();
};

