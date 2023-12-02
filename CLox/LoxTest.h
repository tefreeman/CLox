#pragma once
#include <string>
#include "Scanner.h"
#include "LoxTypes.h"
#include <vector>
#include <sstream>
#include "LoxConsole.h"
#include <regex>
#include <algorithm>

namespace ansi {
  template < class CharT, class Traits >
  constexpr
    std::basic_ostream< CharT, Traits >& reset(std::basic_ostream< CharT, Traits >& os)
  {
    return os << "\033[0m";
  }

  template < class CharT, class Traits >
  constexpr
    std::basic_ostream< CharT, Traits >& foreground_black(std::basic_ostream< CharT, Traits >& os)
  {
    return os << "\033[30m";
  }

  template < class CharT, class Traits >
  constexpr
    std::basic_ostream< CharT, Traits >& foreground_red(std::basic_ostream< CharT, Traits >& os)
  {
    return os << "\033[31m";
  }

  template < class CharT, class Traits >
  constexpr
    std::basic_ostream< CharT, Traits >& foreground_green(std::basic_ostream< CharT, Traits >& os)
  {
    return os << "\033[32m";
  }
} // ansi


class LoxTest {

  // trim from start (in place)
  static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
      return !std::isspace(ch);
      }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
      return !std::isspace(ch);
      }).base(), s.end());
  }
  // trim from both ends (in place)
  static inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
  }
private:
  std::vector<std::string> expectedVec_;
  int failedTests_ = 0;
  bool testsRun_ = false;
  bool nothingFlag_ = false;
  std::string name_;

  std::string getTextAfterColon(const std::string& str) {
    size_t colonPos = str.find(':');
    if (colonPos != std::string::npos) {
      // Add 1 to colonPos to start after the colon character
      return str.substr(colonPos + 1);
    }
    else {
      // Return an empty string if no colon is found
      return "";
    }
  }

  // a = expected, b == actual
  bool isEqv(std::string& a, std::string& b) {
    std::transform(a.begin(), a.end(), a.begin(),
      [](unsigned char c) { return std::tolower(c); });

    std::transform(b.begin(), b.end(), b.begin(),
      [](unsigned char c) { return std::tolower(c); });

    if (a == b) {
      return true;
    }
    if (a == "true" && b == "1") return true;
    if (a == "false" && b == "0") return true;

    if (isNumber(a) && isNumber(b)) {
      double numA = std::stod(a);
      double numB = std::stod(b);

      return numA == numB;
    }

    if (a.find("error") != std::string::npos && b.find("error") != std::string::npos) {
      
      if (a.find("[line") != std::string::npos && b.find("[line") != std::string::npos) {
        
        std::regex pattern("\\[line (\\d+)\\]");
        std::smatch matches;

        std::string aLineNum;
        std::string bLineNum;

        if (std::regex_search(a, matches, pattern) && matches.size() > 1) {
          aLineNum = matches[1].str();
        }
        if (std::regex_search(b, matches, pattern) && matches.size() > 1) {
          bLineNum = matches[1].str();
        }

       return aLineNum != bLineNum;


      }

      return true;
    }
    std::string err_msg = getTextAfterColon(a);
    trim(err_msg);

    if(err_msg != "" && b.find(err_msg) != std::string::npos) return true;

    return false;
  }

  // taken from https://java2blog.com/check-if-string-is-number-cpp/
  bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double val;
    iss >> val;
    return iss.eof() && !iss.fail();
  }

  bool doubleIsInt(double d) {
    return floor(d) == ceil(d);
  }

  // extracts comment or returns empty string
  std::string extractComment(const std::string& str) {
    bool inQuotes = false;
    size_t length = str.length();

    for (size_t i = 0; i < length; ++i) {
      // Toggle inQuotes when a quote is found
      if (str[i] == '\"' || str[i] == '\'') {
        inQuotes = !inQuotes;
      }

      // Check for '//' outside of quotes
      if (!inQuotes && i < length - 1 && str[i] == '/' && str[i + 1] == '/') {
        return str.substr(i + 2);
      }
    }

    return ""; // Return empty string if no comment is found
  }

  bool parseTest(std::string& comment, std::string find_str , bool doRemove) {

      std::size_t found = comment.find(find_str);
      std::string expected;

      if (found != std::string::npos) {
        if (doRemove)
          expected = comment.substr(found + find_str.length());
        else
          expected = comment.substr(found);
        

        trim(expected);
        
        expectedVec_.push_back(expected);
        return true;
      }
      return false;
    }
  public:
    LoxTest(std::string name, std::string text) {
      name_ = name;


      std::string line;
      std::istringstream iss(text);

      while (std::getline(iss, line)) {
        std::string comment = extractComment(line);

        // to lower
        std::transform(comment.begin(), comment.end(), comment.begin(),
          [](unsigned char c) { return std::tolower(c); });

        // no valid comment found skip to next line
        if (comment == "") {
          continue;
        }

       if(parseTest(comment, "expect:", true)) continue;
       if(parseTest(comment, "error", false)) continue;
       if (parseTest(comment, "~nothing~", false)) {
         nothingFlag_ = true;
         break;
       }
     }
  }
    

    void printResults() {
      if (!testsRun_) {
        std::cout << ansi::foreground_red << "No tests run!" << ansi::reset << std::endl;
        return;
      }

      if (failedTests_ == 0) {
         std::cout << ansi::foreground_green << expectedVec_.size() << "/" << expectedVec_.size()  << " Test passed!" << ansi::reset << std::endl;
      }
      else {
        std::cout << ansi::foreground_red << expectedVec_.size() << "/" << failedTests_ << " Test failed!" << ansi::reset << std::endl;
      }
    }

    void printStart() {
      std::cout << "----------------------------------------------------" << std::endl;
      std::cout << "Running " << name_ <<  " Tests " << std::endl;
    }

    void test() {
      // For test files that expect nothing to happen flagged via //  ~NOTHING~
      if (nothingFlag_) {
        testsRun_ = true;
        return;
      }

      if (expectedVec_.size() == 0) {
        std::cout << ansi::foreground_red << "No expected values found!" << ansi::reset << std::endl;
        return;
      }
      //  console history is 1 longer than expected because of the prompt
      if (expectedVec_.size() != lox_console::_console_history.size() - 1) {
        std::cout << ansi::foreground_red << "Cannot Parse " << name_ << ": " << " Each print needs an '// expect: value'  |  Detected:  " << lox_console::_console_history.size()-1 << "/" << expectedVec_.size() << "." << ansi::reset << std::endl;
        return;
      }

      testsRun_ = true;

      for (int i = 0; i < expectedVec_.size(); i++) {
        std::string expected = expectedVec_[i];
        std::string actual = lox_console::get(i);
        if (!isEqv(expected, actual)) {
          failedTests_++;
          std::cout << ansi::foreground_red;
          std::cout << "Test failed:   ";
          std::cout << "Expected: " << expected;
          std::cout << "  Actual: " << actual;
          std::cout << ansi::reset << std::endl;
        }
      }

    }

};