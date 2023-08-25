#include "types.hpp"
#include "error.hpp"
#include "env.hpp"
#include "printer.hpp"

#include <iostream>

using std::cout;
using std::endl;

MalType EVAL(const MalType& input, Env env);

MalType MFalse = MalType(new MalFalse());
MalType MTrue = MalType(new MalTrue());
MalType MNil = MalType(new MalNil());

MalInt::MalInt(int value)
{
  value_ = value;
}

string MalInt::getString(bool _)
{
  return std::to_string(value_);
}

int MalInt::operator+(const MalInt& other) const
{
  return value_ + other.value_;
}

int MalInt::operator-(const MalInt& other) const
{
  return value_ - other.value_;
}

int MalInt::operator*(const MalInt& other) const
{
  return value_ * other.value_;
}

int MalInt::operator/(const MalInt& other) const
{
  return value_ / other.value_;
}

bool MalInt::operator>(const MalInt& other) const
{
  return value_ > other.value_;
}

bool MalInt::operator>=(const MalInt& other) const
{
  return !((*this) < other);
}

bool MalInt::operator<(const MalInt& other) const
{
  return value_ < other.value_;
}

bool MalInt::operator<=(const MalInt& other) const
{
  return !((*this) > other);
}

bool MalInt::equals(const MalType& other) const
{
  if(typeid(*other) != typeid(MalInt)) return false;

  auto* otherInt = dynamic_cast<MalInt*>(&*other);

  return value_ == otherInt->value_;
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

bool MalSymbol::equals(const MalType& other) const
{
  if(typeid(*other) != typeid(MalSymbol)) return false;

  auto* otherSymbol = dynamic_cast<MalSymbol*>(&*other);

  return symbol_ == otherSymbol->symbol_;
}

MalEnumerable::MalEnumerable(vector<MalType> elements)
{
  elements_ = elements;
}

bool MalEnumerable::equals(const MalType& other) const
{
  if(typeid(*other) != typeid(MalEnumerable)) return false;

  auto* otherEnumerable = dynamic_cast<MalEnumerable*>(&*other);

  if(size() != otherEnumerable->size()) return false;

  for(size_t i = 0; i < size(); ++i)
  {
    if(!elements_[i]->equals((*otherEnumerable)[i])) return false;
  }

  return true;
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

bool MalString::equals(const MalType& other) const
{
  if(typeid(*other) != typeid(MalString)) return false;

  auto* otherString = dynamic_cast<MalString*>(&*other);

  return value_ == otherString->value_;
}

string MalNil::getString(bool _)
{
  return "nil";
}

bool MalNil::equals(const MalType& other) const
{
  return other == MNil;
}

string MalTrue::getString(bool _)
{
  return "true";
}

bool MalTrue::equals(const MalType& other) const
{
  return other == MTrue;
}

string MalFalse::getString(bool _)
{
  return "false";
}

bool MalFalse::equals(const MalType& other) const
{
  return other == MFalse;
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

bool MalKeyword::equals(const MalType& other) const
{
  if(typeid(*other) != typeid(MalKeyword)) return false;

  auto* otherString = dynamic_cast<MalKeyword*>(&*other);

  return keyword_ == otherString->keyword_;
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

string MalOperation::getString(bool _)
{
  return "";
}

MalType MalAddOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return MalType(new MalInt((*a) + (*b)));
}

MalType MalMultOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return MalType(new MalInt((*a) * (*b)));
}

MalType MalSubOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return MalType(new MalInt((*a) - (*b)));
}

MalType MalDivOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return MalType(new MalInt((*a) / (*b)));
}

MalFunction::MalFunction(const vector<MalType>& bindings, const MalType& body, const Env& baseEnv)
{
  bindings_ = bindings;
  body_ = body;
  baseEnv_ = baseEnv;
}

string MalFunction::getString(bool _)
{
  return "#<function>";
}

MalType MalFunction::apply(const vector<MalType>& args)
{
  return EVAL(body_, Env(new EnvData(baseEnv_, bindings_, args)));
}

MalType MalPrnOperation::apply(const vector<MalType>& args)
{
  cout << Printer::prStr(args[0], true) << endl;

  return MNil;
}

MalType MalListOperation::apply(const vector<MalType>& args)
{
  return MalType(new MalList(args));
}

MalType MalIsListOperation::apply(const vector<MalType>& args)
{
  return typeid(*args[0]) == typeid(MalList) ? MTrue : MFalse;
}

MalType MalIsEmptyOperation::apply(const vector<MalType>& args)
{
  auto* list = dynamic_cast<MalList*>(&*args[0]);

  return list->isEmpty() ? MTrue : MFalse;
}

MalType MalCountOperation::apply(const vector<MalType>& args)
{
  if(typeid(*args[0]) != typeid(MalList)) return MalType(new MalInt(0));

  auto* list = dynamic_cast<MalList*>(&*args[0]);

  return MalType(new MalInt(static_cast<int>(list->size())));
}

MalType MalEqualsOperation::apply(const vector<MalType>& args)
{
  return args[0]->equals(args[1]) ? MTrue : MFalse;
}

MalType MalGTOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return (*a) > (*b) ? MTrue : MFalse;
}

MalType MalGTEOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return (*a) >= (*b) ? MTrue : MFalse;
}

MalType MalLTOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return (*a) < (*b) ? MTrue : MFalse;
}

MalType MalLTEOperation::apply(const vector<MalType>& args)
{
  auto a = dynamic_cast<MalInt*>(&*args[0]);
  auto b = dynamic_cast<MalInt*>(&*args[1]);

  return (*a) <= (*b) ? MTrue : MFalse;
}
