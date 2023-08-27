#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>

using std::string;
using std::vector;
using std::shared_ptr;
using std::map;
using std::function;

class MalTypeData;
typedef shared_ptr<MalTypeData> MalType;

class EnvData;
typedef shared_ptr<EnvData> Env;

extern MalType MFalse;
extern MalType MTrue;
extern MalType MNil;

class MalTypeData
{
public:
  virtual string getString(bool printReadably) = 0;

  virtual bool equals(const MalType& other) const { return false; }
};

class MalInt : public MalTypeData
{
  int value_;

public:
  MalInt(int value);

  virtual string getString(bool printReadably) override;

  int operator+(const MalInt& other) const;
  int operator-(const MalInt& other) const;
  int operator*(const MalInt& other) const;
  int operator/(const MalInt& other) const;
  bool operator>(const MalInt& other) const;
  bool operator>=(const MalInt& other) const;
  bool operator<(const MalInt& other) const;
  bool operator<=(const MalInt& other) const;

  int operator*() const;

  virtual bool equals(const MalType& other) const override;
};

class MalSymbol : public MalTypeData
{
  string symbol_;

public:
  MalSymbol(string symbol);

  string getSymbol() const;

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;
};

class MalEnumerable : public MalTypeData
{
protected:
  vector<MalType> elements_;

public:
  MalEnumerable(vector<MalType> elements);

  virtual string getString(bool printReadably) override = 0;

  vector<MalType>::iterator begin() { return elements_.begin(); }
  vector<MalType>::iterator end() { return elements_.end(); }

  bool isEmpty() const { return elements_.empty(); }
  size_t size() const { return elements_.size(); }

  MalType operator[](const int& index) { return elements_[index]; }

  virtual bool equals(const MalType& other) const override;
};

class MalList : public MalEnumerable
{
public:
  MalList(vector<MalType> elements): MalEnumerable(elements) {}

  virtual string getString(bool printReadably) override;
};

class MalString : public MalTypeData
{
  string value_;

public:
  MalString(const string& value);

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;

  string operator*();
};

class MalNil : public MalTypeData
{
public:
  MalNil() = default;

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;
};

class MalTrue : public MalTypeData
{
public:
  MalTrue() = default;

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;
};

class MalFalse : public MalTypeData
{
public:
  MalFalse() = default;

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;
};

class MalVector : public MalEnumerable
{
public:
  MalVector(vector<MalType> elements): MalEnumerable(elements) {}

  virtual string getString(bool printReadably) override;
};

class MalKeyword : public MalTypeData
{
  string keyword_;

public:
  MalKeyword(const string& keyword);

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;
};

class MalHashMap : public MalTypeData
{
  map<string, MalType> map_;

public:
  MalHashMap(vector<MalType> elements);

  virtual string getString(bool printReadably) override;

  map<string, MalType>::iterator begin() { return map_.begin(); }
  map<string, MalType>::iterator end() { return map_.end(); }

  MalType& operator[](const string& key);
};

class MalOperation : public MalTypeData
{
public:
  MalOperation() = default;

  virtual string getString(bool printReadably) override;

  virtual MalType apply(const vector<MalType>& args) = 0;
};

class MalAddOperation : public MalOperation
{
public:
  MalAddOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalMultOperation : public MalOperation
{
public:
  MalMultOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalSubOperation : public MalOperation
{
public:
  MalSubOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalDivOperation : public MalOperation
{
public:
  MalDivOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalFunction : public MalOperation
{
  vector<MalType> bindings_;
  MalType body_;
  Env baseEnv_;
  bool isMacro_;

public:
  MalFunction(const vector<MalType>& bindings, const MalType& body, const Env& baseEnv, const bool& isMacro = false);

  virtual string getString(bool printReadably) override;

  MalType apply(const vector<MalType>& args);

  MalType getBody() const;
  Env makeEnv(const vector<MalType>& args) const;

  void makeMacro();

  bool isMacro() const;
};

class MalPrnOperation : public MalOperation
{
public:
  MalPrnOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalListOperation : public MalOperation
{
public:
  MalListOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalIsListOperation : public MalOperation
{
public:
  MalIsListOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalIsEmptyOperation : public MalOperation
{
public:
  MalIsEmptyOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalCountOperation : public MalOperation
{
public:
  MalCountOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalEqualsOperation : public MalOperation
{
public:
  MalEqualsOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalGTOperation : public MalOperation
{
public:
  MalGTOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalGTEOperation : public MalOperation
{
public:
  MalGTEOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalLTOperation : public MalOperation
{
public:
  MalLTOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalLTEOperation : public MalOperation
{
public:
  MalLTEOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalPrStrOperation : public MalOperation
{
public:
  MalPrStrOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalStrOperation : public MalOperation
{
public:
  MalStrOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalPrintlnOperation : public MalOperation
{
public:
  MalPrintlnOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalReadStringOperation : public MalOperation
{
public:
  MalReadStringOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalSlurpOperation : public MalOperation
{
public:
  MalSlurpOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalEvalOperation : public MalOperation
{
public:
  MalEvalOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalAtom : public MalTypeData
{
  MalType ref_;

public:
  MalAtom(const MalType& ref);

  virtual string getString(bool printReadably) override;

  virtual bool equals(const MalType& other) const override;

  MalType operator*();

  void setRef(const MalType& ref);
};

class MalAtomOperation : public MalOperation
{
public:
  MalAtomOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalIsAtomOperation : public MalOperation
{
public:
  MalIsAtomOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalDerefOperation : public MalOperation
{
public:
  MalDerefOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalResetOperation : public MalOperation
{
public:
  MalResetOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalSwapOperation : public MalOperation
{
public:
  MalSwapOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalConsOperation : public MalOperation
{
public:
  MalConsOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalConcatOperation : public MalOperation
{
public:
  MalConcatOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalVecOperation : public MalOperation
{
public:
  MalVecOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalNthOperation : public MalOperation
{
public:
  MalNthOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalFirstOperation : public MalOperation
{
public:
  MalFirstOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};

class MalRestOperation : public MalOperation
{
public:
  MalRestOperation() = default;

  virtual MalType apply(const vector<MalType>& args) override;
};
