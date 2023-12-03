#pragma once
#include <string>
#include <vector>
#include <sstream>
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

namespace util {
  // trim from start (in place)
  inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
      return !std::isspace(ch);
      }));
  }

  // trim from end (in place)
  inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
      return !std::isspace(ch);
      }).base(), s.end());
  }
  static inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
  }

  inline std::string getTextAfterChar(const std::string& str, const char c) {
    size_t charPos = str.find(c);
    if (charPos != std::string::npos) {
      // Add 1 to colonPos to start after the colon character
      return str.substr(charPos + 1);
    }
    else {
      // Return an empty string if no colon is found
      return "";
    }
  }

  // taken from https://java2blog.com/check-if-string-is-number-cpp/
  inline bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double val;
    iss >> val;
    return iss.eof() && !iss.fail();
  }

  inline bool doubleIsInt(double d) {
    return floor(d) == ceil(d);
  }

  // extracts comment or returns empty string
  // checks for comments outside of quotes
  inline std::string extractComment(const std::string& str) {
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

  inline std::string removeLineTags(const std::string& input) {
    // Regular expression to match [line x] pattern
    std::regex lineTagPattern("\\[line \\d+\\]");

    // Replace occurrences of the pattern with an empty string
    std::string result = std::regex_replace(input, lineTagPattern, "");

    return result;
  }
}