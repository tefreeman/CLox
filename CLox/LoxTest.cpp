#include "LoxConsole.h"
#include "LoxTest.h"
#include "utility/StrFuncs.h"
#include <algorithm>
#include <regex>
// a = expected, b == actual

using namespace util;
bool LoxTest::IsEquivalent(std::string& expected, std::string& consoleOutput) {
  trim(expected);
  trim(consoleOutput);
  // to lower 
  std::transform(expected.begin(), expected.end(), expected.begin(),
    [](unsigned char c) { return std::tolower(c); });

  std::transform(consoleOutput.begin(), consoleOutput.end(), consoleOutput.begin(),
    [](unsigned char c) { return std::tolower(c); });


  if (expected == consoleOutput) {
    return true;
  }
  // if expected is true and console output is 1
  if (expected == "true" && consoleOutput == "1") return true;
  // if expected is false and console output is 0
  if (expected == "false" && consoleOutput == "0") return true;

  // if both are numbers convert to double and compare
  if (isNumber(expected) && isNumber(consoleOutput)) {
    double numA = std::stod(expected);
    double numB = std::stod(consoleOutput);

    return numA == numB;
  }
  // We find error in both strings
  if (expected.find("error") != std::string::npos && consoleOutput.find("error") != std::string::npos) {
    // Check if we have a line number then we try to match line number
    if (expected.find("[line") != std::string::npos && consoleOutput.find("[line") != std::string::npos) {

      std::regex pattern("\\[line (\\d+)\\]");
      std::smatch matches;

      std::string aLineNum;
      std::string bLineNum;

      if (std::regex_search(expected, matches, pattern) && matches.size() > 1) {
        aLineNum = matches[1].str();
      }
      if (std::regex_search(consoleOutput, matches, pattern) && matches.size() > 1) {
        bLineNum = matches[1].str();
      }

      return aLineNum != bLineNum;


    }
    // else remove the line number [line x] clause
    consoleOutput = removeLineTags(consoleOutput);

    std::string onlyExpectedErrorMsg = getTextAfterChar(expected, ':');
    trim(onlyExpectedErrorMsg);


    if (consoleOutput.find(onlyExpectedErrorMsg) != std::string::npos) return true;
  }
  //

  return false;
}

bool LoxTest::parseTestFile(std::string& comment, std::string find_str, bool doRemove) {

  std::size_t found = comment.find(find_str);
  std::string expected;

  if (found != std::string::npos) {
    if (doRemove)
      expected = comment.substr(found + find_str.length());
    else
      expected = comment.substr(found);


    trim(expected);

    expectedOutputs_.push_back(expected);
    return true;
  }
  return false;
}

void LoxTest::extractExpectedOutputs(std::string& testText) {

  std::string line;
  std::istringstream iss(testText);

  while (std::getline(iss, line)) {
    std::string comment = extractComment(line);

    // to lower
    std::transform(comment.begin(), comment.end(), comment.begin(),
      [](unsigned char c) { return std::tolower(c); });

    // no valid comment found skip to next line
    if (comment == "") {
      continue;
    }

    if (parseTestFile(comment, "expect:", true)) continue;
    if (parseTestFile(comment, "error", false)) continue;
    if (parseTestFile(comment, "~nothing~", false)) {
      expectedNothing_ = true;
      break;
    }
  }
}

LoxTest::LoxTest(std::string name, std::string& text, bool redirectOutput) {
  testName_ = name;
  extractExpectedOutputs(text);
  redirectOutput_ = redirectOutput;
}

void LoxTest::printResults() {
  if (!didTestsRun_) {
    if (!redirectOutput_)
      std::cout << ansi::foreground_red;

    std::cout << "No tests run!" << std::endl;

    if (!redirectOutput_)
      std::cout << ansi::reset;
    return;
  }

  if (failedTests_ == 0) {
    if (!redirectOutput_)
      std::cout << ansi::foreground_green;

    std::cout << expectedOutputs_.size() << "/" << expectedOutputs_.size() << " Test passed!" << std::endl;

    if (!redirectOutput_)
      std::cout << ansi::reset;
  }
  else {

    if (!redirectOutput_)
      std::cout << ansi::foreground_red;

    std::cout << expectedOutputs_.size() << "/" << failedTests_ << " Test failed!" << std::endl;

    if (!redirectOutput_)
      std::cout << ansi::reset;

  }
}

void LoxTest::printStart() {
  std::cout << "----------------------------------------------------" << std::endl;
  std::cout << "Running " << testName_ << " Tests " << std::endl;
}

void LoxTest::validate() {
  // For test files that expect nothing to happen flagged via //  ~NOTHING~
  if (expectedNothing_) {
    didTestsRun_ = true;
    return;
  }

  if (expectedOutputs_.size() == 0) {
    if (!redirectOutput_)
      std::cout << ansi::foreground_red;

    std::cout << "No expected values found!" << std::endl;

    if (!redirectOutput_)
      std::cout << ansi::reset;
    return;
  }
  //  console history is 1 longer than expected because of the prompt
  if (expectedOutputs_.size() != lox_console::_console_history.size() - 1) {
    if (!redirectOutput_)
      std::cout << ansi::foreground_red;

    std::cout << "Cannot Parse " << testName_ << ": " << " Each print needs an '// expect: value'  |";
    std::cout << " Detected:  " << lox_console::_console_history.size() - 1 << "/" << expectedOutputs_.size() << "." << std::endl;

    if (!redirectOutput_)
      std::cout << ansi::reset;

    return;
  }

  didTestsRun_ = true;

  for (int i = 0; i < expectedOutputs_.size(); i++) {
    std::string expected = expectedOutputs_[i];
    std::string actual = lox_console::get(i);
    if (!IsEquivalent(expected, actual)) {
      failedTests_++;

      if (!redirectOutput_)
        std::cout << ansi::foreground_red;

      std::cout << "Test failed:   ";
      std::cout << "Expected: " << expected;
      std::cout << "  Actual: " << actual;

      if (!redirectOutput_)
        std::cout << ansi::reset << std::endl;
    }
    else {

      if (!redirectOutput_)
        std::cout << ansi::foreground_green;

      std::cout << "Test passed:   ";
      std::cout << "Expected: " << expected;
      std::cout << "  Actual: " << actual << std::endl;

      if (!redirectOutput_)
        std::cout << ansi::reset;
    }

  }
}
