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

class MalTypeData
{
public:
  virtual string getString(bool printReadably) = 0;
};

typedef shared_ptr<MalTypeData> MalType;

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
};

class MalSymbol : public MalTypeData
{
  string symbol_;

public:
  MalSymbol(string symbol);

  string getSymbol() const;

  virtual string getString(bool printReadably) override;
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
};

class MalNil : public MalTypeData
{
public:
  MalNil() = default;

  virtual string getString(bool printReadably) override;
};

class MalTrue : public MalTypeData
{
public:
  MalTrue() = default;

  virtual string getString(bool printReadably) override;
};

class MalFalse : public MalTypeData
{
public:
  MalFalse() = default;

  virtual string getString(bool printReadably) override;
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

  virtual MalType apply(vector<MalType> args) = 0;
};

class MalAddOperation : public MalOperation
{
public:
  MalAddOperation() = default;

  virtual MalType apply(vector<MalType> args) override;
};

class MalMultOperation : public MalOperation
{
public:
  MalMultOperation() = default;

  virtual MalType apply(vector<MalType> args) override;
};

class MalSubOperation : public MalOperation
{
public:
  MalSubOperation() = default;

  virtual MalType apply(vector<MalType> args) override;
};

class MalDivOperation : public MalOperation
{
public:
  MalDivOperation() = default;

  virtual MalType apply(vector<MalType> args) override;
};
