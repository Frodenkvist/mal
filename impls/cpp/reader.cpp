#include "reader.hpp"
#include <regex>
#include "types.hpp"
#include <memory>
#include "debug.hpp"
#include "error.hpp"

using std::regex;
using std::smatch;
using std::regex_search;
using std::shared_ptr;

static const regex whitespaceRegex("[\\s,]+|;.*");
static const regex tokenRegexes[] = {
  regex("~@"), // capture ~@
  regex("[\\[\\]{}()'`~^@]"), // capture any single []{}()'`~^@
  regex("\"(?:\\\\.|[^\\\\\"])*\"?"), // capture everything between two double quotes
  regex(";.*"), // capture any sequence of characters starting with ;
  regex("[^\\s\\[\\]{}('\"`,;)]*") // capture a sequence of zero or more non special characters
};

Tokenizer::Tokenizer(const string& input)
{
  iter_ = input.begin();
  end_ = input.end();

  nextToken();
}

string Tokenizer::next()
{
  //ASSERT(!eof(), "Tokenizer reading past EOF in next\n");
  if(eof()) throw EOFException("Tokenizer reading past EOF in next\n");

  auto ret = peek();
  nextToken();
  return ret;
}

string Tokenizer::peek() const
{
  if(eof()) throw EOFException("Tokenizer reading past EOF in peek\n");
  //ASSERT(!eof(), "Tokenizer reading past EOF in peek\n");
  return token_;
}

bool Tokenizer::eof() const
{
  return iter_ == end_;
}

void Tokenizer::skipWhitespace()
{
  while(fetchTokenFromRegex(whitespaceRegex))
    iter_ += token_.size();
}

bool Tokenizer::fetchTokenFromRegex(regex regex)
{
  if(eof()) return false;

  smatch match;
  auto flags = std::regex_constants::match_continuous;

  if(!regex_search(iter_, end_, match, regex, flags)) return false;

  token_ = match.str(0);

  return true;
}

void Tokenizer::nextToken()
{
  iter_ += token_.size();

  skipWhitespace();

  if(eof()) return;

  for(const auto& regex : tokenRegexes)
  {
    if(fetchTokenFromRegex(regex)) return;
  }
}

MalType Tokenizer::readStr(const string& input)
{
  auto tokenizer = std::make_shared<Tokenizer>(input);

  return readFrom(tokenizer);
}

MalType Tokenizer::readFrom(const shared_ptr<Tokenizer>& tokenizer)
{
  if(tokenizer->peek() == "(")
  {
    tokenizer->next();
    return readList(tokenizer);
  }

  return readAtom(tokenizer);
}

MalType Tokenizer::readAtom(const shared_ptr<Tokenizer>& tokenizer)
{
  string token = tokenizer->next();

  if(std::all_of(token.begin(), token.end(), ::isdigit))
  {
    return MalType(new MalInt(std::stoi(token)));
  }

  return MalType(new MalSymbol(token));
}

MalType Tokenizer::readList(const shared_ptr<Tokenizer>& tokenizer)
{
  vector<MalType> elements;

  try
  {
    while(tokenizer->peek() != ")")
    {
      elements.push_back(readFrom(tokenizer));
    }
  }
  catch(EOFException& err)
  {
    err.log();
    throw;
  }

  tokenizer->next();

  return MalType(new MalList(elements));
}


