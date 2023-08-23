#include "types.hpp"
#include "error.hpp"

MalInt::MalInt(int value)
{
  value_ = value;
}

string MalInt::getString(bool _)
{
  return std::to_string(value_);
}

MalSymbol::MalSymbol(string symbol)
{
  symbol_ = symbol;
}

string MalSymbol::getSymbol() const
{
  return symbol_;
}

string MalSymbol::getString(bool _)
{
  return symbol_;
}

MalList::MalList(vector<MalType> elements)
{
  elements_ = elements;
}

string MalList::getString(bool printReadably)
{
  string out = "(";

  for(size_t i = 0; i < elements_.size(); ++i)
  {
    if(i != 0) out += " ";
    out += elements_[i]->getString(printReadably);
  }

  out += ")";

  return out;
}

MalString::MalString(const string& value)
{
  value_ = value;
}

string MalString::getString(bool printReadably)
{
  if(!printReadably)
    return value_;

  string readableString;

  readableString.reserve(value_.size() * 2 + 2);

  readableString += '"';

  for(auto itr = value_.begin() + 1, end = value_.end() - 1; itr != end; ++itr)
  {
    char c = *itr;

    if(c == '\n') readableString += "\\n";
    else if(c == '\\') readableString += "\\\\";
    else if(c == '"') readableString += "\\\"";
    else readableString += c;
  }

  readableString += '"';
  readableString.shrink_to_fit();

  return readableString;
}

string MalNil::getString(bool _)
{
  return "nil";
}

string MalTrue::getString(bool _)
{
  return "true";
}

string MalFalse::getString(bool _)
{
  return "false";
}

MalVector::MalVector(vector<MalType> elements)
{
  elements_ = elements;
}

string MalVector::getString(bool printReadably)
{
  string out = "[";

  for(size_t i = 0; i < elements_.size(); ++i)
  {
    if(i != 0) out += " ";
    out += elements_[i]->getString(printReadably);
  }

  out += "]";

  return out;
}

MalKeyword::MalKeyword(const string& keyword)
{
  keyword_ = keyword;
}

string MalKeyword::getString(bool _)
{
  return keyword_;
}

MalHashMap::MalHashMap(vector<MalType> elements)
{
  if(elements.size() % 2 != 0) throw EOFException("Expected equal amount of keys and values");

  for(size_t i = 0; i < elements.size(); i += 2)
  {
    const auto& key = elements[i];
    const auto& value = elements[i + 1];

    if(typeid(*key) != typeid(MalString) && typeid(*key) != typeid(MalKeyword)) throw InvalidKeyException("Invalid key in hashmap");

    map_[key->getString(false)] = value;
  }
}

string MalHashMap::getString(bool printReadably)
{
  string out = "{";

  for(auto itr = map_.begin(); itr != map_.end(); ++itr)
  {
    out += itr->first + " " + itr->second->getString(printReadably) + " ";
  }

  if(out[out.size() - 1] == ' ') out.pop_back();

  out += "}";

  return out;
}

MalType& MalHashMap::operator[](const string& key)
{
  return map_[key];
}
