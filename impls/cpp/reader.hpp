#pragma once

#include <vector>
#include <string>
#include <memory>
#include "types.hpp"
#include <regex>

using std::vector;
using std::string;
using std::shared_ptr;
using std::regex;

class Tokenizer
{
  string token_;
  string::const_iterator iter_;
  string::const_iterator end_;

public:
  Tokenizer(const string& input);

  string next();
  string peek() const;

  bool eof() const;

  static MalType readStr(const string& input);
  static MalType readFrom(const shared_ptr<Tokenizer>& tokenizer);
  static MalType readAtom(const shared_ptr<Tokenizer>& tokenizer);
  static MalType readList(const shared_ptr<Tokenizer>& tokenizer);

private:
  void skipWhitespace();
  void nextToken();
  bool fetchTokenFromRegex(regex regex);
};
