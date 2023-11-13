#include "Scanner.h"
#include "LoxError.h"

using namespace lox_types;

void Scanner::ScanToken()
{
  char c = Advance();
  switch (c) {
    case '(': AddToken(LEFT_PAREN); break;
    case ')': AddToken(RIGHT_PAREN); break;
    case '{': AddToken(LEFT_BRACE); break;
    case '}': AddToken(RIGHT_BRACE); break;
    case ',': AddToken(COMMA); break;
    case '.': AddToken(DOT); break;
    case '-': AddToken(MINUS); break;
    case '+': AddToken(PLUS); break;
    case ';': AddToken(SEMICOLON); break;
    case '*': AddToken(STAR); break;

    // 2 char matching

    case '!':
      AddToken(Match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      AddToken(Match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      AddToken(Match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      AddToken(Match('=') ? GREATER_EQUAL : GREATER);
      break;

    // divison & comment special case

    case '/':
      if (Match('/')) {
        // A comment goes until the end of the line.
        while (Peek() != '\n' && !IsAtEnd()) Advance();
      }
      else {
        AddToken(SLASH);
      }
      break;

      // newlines and white spaces

      case ' ':
        break;
      case '\r':
        break;
      case '\t':
        break;
      case '\n':
        line_++;
        break;


      case '"': 
        StringHandler();
        break;

    default:
      if (IsDigit(c)) {
        NumberHandler();
      }
      else if (IsAlpha(c)) {
        Identifier();
      }
      else {
      lox_error::Error(line_, "Unexpected character.");
      }
      break;
    }
}

bool Scanner::IsAtEnd()
{
  return current_ >= source_.length();
}

char Scanner::Advance()
{
  return source_[current_++];
}

bool Scanner::Match(char expected)
{
  if (IsAtEnd())
    return false;

  if (source_.at(current_) != expected)
    return false;

   current_++;
   return true;
}

char Scanner::PeekNext()
{
  if (current_ + 1 >= source_.length()) return '\0';
  return source_.at(current_ + 1);
}

char Scanner::Peek()
{
  if (IsAtEnd()) return '\0';
  return source_.at(current_);
}

void Scanner::AddToken(TokenType type)
{
  AddToken(type, nullptr);
}

void Scanner::AddToken(TokenType type, Literal literal)
{
  std::string text = source_.substr(start_, current_ - start_);
  tokens_.push_back(Token(type, text, literal, line_));
}

bool Scanner::IsDigit(char c)
{
  return c>= '0' && c <= '9';
}

void Scanner::NumberHandler()
{
  while(IsDigit(Peek())) Advance();

  if (Peek() == '.' && IsDigit(PeekNext())) {
    // Consume the '.'
    Advance();

    while (IsDigit(Peek())) Advance();
  }

  AddToken(NUMBER, std::stod(source_.substr(start_, current_)));
}

void Scanner::Identifier()
{
  while (IsAlphaNumeric(Peek())) Advance();

  std::string text = source_.substr(start_, current_);
  auto type_iter = reserved_keyword_map.find(text);

  TokenType type;
  if (type_iter == reserved_keyword_map.end()) {
    type = IDENTIFIER;
  }
  // Keyword map is std::pair <str, TokenType> so return second
  else {
    type = type_iter->second;
  }

  AddToken(type);
}

bool Scanner::IsAlpha(char c)
{
  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    c == '_';
}

bool Scanner::IsAlphaNumeric(char c)
{
  return IsAlpha(c) || IsDigit(c);
}

void Scanner::StringHandler()
{
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n') line_++;
    Advance();
  }

  if (IsAtEnd()) {
    lox_error::Error(line_, "Unterminated string.");
    return;
  }
  // Go past the closing '"'
  Advance();

  // Trim the surrounding quotes
  int start = start_ + 1;
  int len = current_ - start - 1;

  std::string value = source_.substr(start, len);
  AddToken(STRING, value);
}

std::vector<Token> Scanner::ScanTokens()
{
  while (!IsAtEnd()) {
    // We are at the beginning of the next lexeme
    start_ = current_;
    ScanToken();
    }

    tokens_.push_back(Token(END_OF_FILE, "", true, line_));
    return tokens_;
}
