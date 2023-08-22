#pragma once

#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;

class MalTypeData
{
public:
  virtual string getString() = 0;
};

typedef shared_ptr<MalTypeData> MalType;

class MalInt : public MalTypeData
{
  int value_;

public:
  MalInt(int value);

  virtual string getString() override;
};

class MalSymbol : public MalTypeData
{
  string symbol_;

public:
  MalSymbol(string symbol);

  virtual string getString() override;
};

class MalList : public MalTypeData
{
  vector<MalType> elements_;

public:
  MalList(vector<MalType> elements);

  virtual string getString() override;
};

