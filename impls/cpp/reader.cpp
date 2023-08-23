#include "reader.hpp"
#include <regex>
#include "types.hpp"
#include <memory>
#include "error.hpp"

using std::regex;
using std::smatch;
using std::regex_search;
using std::shared_ptr;

static const regex skipRegexes[] = {
  regex("[\\s,]+"), // whitespaces
  regex(";.*")      // comments
};
static const regex tokenRegexes[] = {
  regex("~@"), // capture ~@
  regex("[\\[\\]{}()'`~^@]"), // capture any single []{}()'`~^@
  regex("\"(?:\\\\.|[^\\\\\"])*\"?"), // capture everything between two double quotes
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
  if(eof()) throw EOFException("Tokenizer reading past EOF in next\n");

  auto ret = peek();
  nextToken();
  return ret;
}

string Tokenizer::peek() const
{
  if(eof()) throw EOFException("Tokenizer reading past EOF in peek\n");
  return token_;
}

bool Tokenizer::eof() const
{
  return iter_ == end_;
}

void Tokenizer::skipWhitespace()
{
  bool foundMatch = true;
  while(foundMatch)
  {
    foundMatch = false;
    for(auto regex : skipRegexes)
    {
      if(fetchTokenFromRegex(regex))
      {
        iter_ += token_.size();
        foundMatch = true;
      }
    }
  }
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
    return readList(tokenizer, ")");
  }

  if(tokenizer->peek() == "[")
  {
    tokenizer->next();
    return readList(tokenizer, "]");
  }

  if(tokenizer->peek() == "{")
  {
    tokenizer->next();
    return readList(tokenizer, "}");
  }

  return readAtom(tokenizer);
}

MalType Tokenizer::readAtom(const shared_ptr<Tokenizer>& tokenizer)
{
  string token = tokenizer->next();

  if(token[0] == '"')
  {
    if(token.size() == 1 || token[token.size() - 1] != '"')
    {
      throw EOFException("missing \"");
    }

    string unescapedToken;
    unescapedToken.reserve(token.size());
    unescapedToken += '"';

    for(auto itr = token.begin() + 1, end = token.end() - 1; itr != end; ++itr)
    {
      char c = *itr;

      if(c == '\\')
      {
        ++itr;
        if(itr == end)
        {
          throw EOFException("reading out of bounds when unescaping");
        }

        switch(*itr)
        {
        case '\\':
          unescapedToken += '\\';
          break;
        case '"':
          unescapedToken += '"';
          break;
        case 'n':
          unescapedToken += '\n';
          break;

        default:
          unescapedToken += c;
        }

        continue;
      }

      unescapedToken += c;
    }

    unescapedToken += '"';

    unescapedToken.shrink_to_fit();

    return MalType(new MalString(unescapedToken));
  }

  if(token[0] == ':')
  {
    return MalType(new MalKeyword(token));
  }

  if(token == "nil")
  {
    return MalType(new MalNil());
  }

  if(token == "true")
  {
    return MalType(new MalTrue());
  }

  if(token == "false")
  {
    return MalType(new MalFalse());
  }

  if(std::all_of(token.begin(), token.end(), ::isdigit))
  {
    return MalType(new MalInt(std::stoi(token)));
  }


  if(token == "'")
  {
    return MalType(new MalList({MalType(new MalSymbol("quote")), readFrom(tokenizer)}));
  }

  if(token == "`")
  {
    return MalType(new MalList({MalType(new MalSymbol("quasiquote")), readFrom(tokenizer)}));
  }

  if(token == "~")
  {
    return MalType(new MalList({MalType(new MalSymbol("unquote")), readFrom(tokenizer)}));
  }

  if(token == "~@")
  {
    return MalType(new MalList({MalType(new MalSymbol("splice-unquote")), readFrom(tokenizer)}));
  }

  if(token == "@")
  {
    return MalType(new MalList({MalType(new MalSymbol("deref")), readFrom(tokenizer)}));
  }

  return MalType(new MalSymbol(token));
}

MalType Tokenizer::readList(const shared_ptr<Tokenizer>& tokenizer, const string& end)
{
  vector<MalType> elements;

  while(tokenizer->peek() != end)
  {
    elements.push_back(readFrom(tokenizer));
  }
  tokenizer->next();

  if(end == ")")
  {
    return MalType(new MalList(elements));
  }

  if(end == "]")
  {
    return MalType(new MalVector(elements));
  }

  if(end == "}")
  {
    return MalType(new MalHashMap(elements));
  }

  throw std::exception();
}


