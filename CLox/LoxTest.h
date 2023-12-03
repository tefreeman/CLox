#pragma once
#include <string>
#include <vector>
#include <sstream>



class LoxTest {

private:
  std::vector<std::string> expectedOutputs_;
  int failedTests_ = 0;
  bool didTestsRun_ = false;
  bool expectedNothing_ = false;
  std::string testName_;


  // a = expected, b == actual
  bool IsEquivalent(std::string& expected, std::string& consoleOutput);
  bool parseTestFile(std::string& comment, std::string find_str , bool doRemove);
  void extractExpectedOutputs(std::string& testText);

  public:
    LoxTest(std::string name, std::string& text);
    

    void printResults();
    void printStart();
    // Compares LoxConsole against expected outputs
    void validate();

};