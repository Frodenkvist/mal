#include "types.hpp"
#include "error.hpp"
#include "env.hpp"
#include "printer.hpp"
#include "reader.hpp"

#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

MalType EVAL(MalType input, Env env);

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
  auto* otherEnumerable = dynamic_cast<MalEnumerable*>(&*other);

  if(!otherEnumerable) return false;

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

  for(auto itr = value_.begin(), end = value_.end(); itr != end; ++itr)
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

string MalString::operator*()
{
  return value_;
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
  keyword_ = '\xff' + keyword;
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
    const auto& value = elements[i + 1];

    if(auto* stringKey = dynamic_cast<MalString*>(&*elements[i]))
    {
      map_['"' + stringKey->getString(false) + '"'] = value;
    }
    else if (auto* keywordKey = dynamic_cast<MalKeyword*>(&*elements[i]))
    {
      map_[keywordKey->getString(false)] = value;
    }
    else
    {
     throw InvalidKeyException("Invalid key in hashmap");
    }
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
  return EVAL(body_, makeEnv(args));
}

MalType MalFunction::getBody() const
{
  return body_;
}

Env MalFunction::makeEnv(const vector<MalType>& args) const
{
  return Env(new EnvData(baseEnv_, bindings_, args));
}

MalType MalPrnOperation::apply(const vector<MalType>& args)
{
  string out;

  for(auto mal : args)
  {
    if(out != "") out += ' ';
    out += Printer::prStr(mal, true);
  }

  cout << out << endl;

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
  auto* enumerable = dynamic_cast<MalEnumerable*>(&*args[0]);

  return enumerable->isEmpty() ? MTrue : MFalse;
}

MalType MalCountOperation::apply(const vector<MalType>& args)
{
  auto* enumerable = dynamic_cast<MalEnumerable*>(&*args[0]);

  if(!enumerable) return MalType(new MalInt(0));

  return MalType(new MalInt(static_cast<int>(enumerable->size())));
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

MalType MalPrStrOperation::apply(const vector<MalType>& args)
{
  string out;

  for(auto itr = args.begin(); itr != args.end(); ++itr)
  {
    if(out != "") out += " ";
    out += Printer::prStr(*itr, true);
  }

  return MalType(new MalString(out));
}

MalType MalStrOperation::apply(const vector<MalType>& args)
{
  string out;

  for(auto itr = args.begin(); itr != args.end(); ++itr)
  {
    out += Printer::prStr(*itr);
  }

  return MalType(new MalString(out));
}

MalType MalPrintlnOperation::apply(const vector<MalType>& args)
{
  string out;

  for(auto mal : args)
  {
    if(out != "") out += ' ';
    out += Printer::prStr(mal);
  }

  cout << out << endl;

  return MNil;
}

MalType MalReadStringOperation::apply(const vector<MalType>& args)
{
  auto* malString = dynamic_cast<MalString*>(&*args[0]);

  return Tokenizer::readStr(*(*malString));
}

MalType MalSlurpOperation::apply(const vector<MalType>& args)
{
  auto* malString = dynamic_cast<MalString*>(&*args[0]);

  ifstream file((*(*malString)));

  string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

  return MalType(new MalString(content));
}

MalType MalEvalOperation::apply(const vector<MalType>& args)
{
  return EVAL(args[0], nullptr);
}

MalAtom::MalAtom(const MalType& ref)
{
  ref_ = ref;
}

string MalAtom::getString(bool printReadably)
{
  return "(atom " + ref_->getString(printReadably) + ")";
}

bool MalAtom::equals(const MalType& other) const
{
  auto* otherAtom = dynamic_cast<MalAtom*>(&*other);
  if(!otherAtom) return false;

  return ref_->equals(otherAtom->ref_);
}

MalType MalAtom::operator*()
{
  return ref_;
}

void MalAtom::setRef(const MalType& ref)
{
  ref_ = ref;
}

MalType MalAtomOperation::apply(const vector<MalType>& args)
{
  return MalType(new MalAtom(args[0]));
}

MalType MalIsAtomOperation::apply(const vector<MalType>& args)
{
  return !!dynamic_cast<MalAtom*>(&*args[0]) ? MTrue : MFalse;
}

MalType MalDerefOperation::apply(const vector<MalType>& args)
{
  return *(*dynamic_cast<MalAtom*>(&*args[0]));
}

MalType MalResetOperation::apply(const vector<MalType>& args)
{
  auto* atom = dynamic_cast<MalAtom*>(&*args[0]);

  atom->setRef(args[1]);

  return args[1];
}

MalType MalSwapOperation::apply(const vector<MalType>& args)
{
  auto* atom = dynamic_cast<MalAtom*>(&*args[0]);
  auto* operation = dynamic_cast<MalOperation*>(&*args[1]);

  vector<MalType> operationArgs = { *(*atom) };

  for(size_t i = 2; i < args.size(); ++i)
  {
    operationArgs.push_back(args[i]);
  }

  atom->setRef(operation->apply(operationArgs));

  return *(*atom);
}
