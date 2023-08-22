#include "types.hpp"

MalInt::MalInt(int value)
{
  value_ = value;
}

string MalInt::getString()
{
  return std::to_string(value_);
}

MalSymbol::MalSymbol(string symbol)
{
  symbol_ = symbol;
}

string MalSymbol::getString()
{
  return symbol_;
}

MalList::MalList(vector<MalType> elements)
{
  elements_ = elements;
}

string MalList::getString()
{
  string out = "(";

  for(size_t i = 0; i < elements_.size(); ++i)
  {
    if(i != 0) out += " ";
    out += elements_[i]->getString();
  }

  out += ")";

  return out;
}
