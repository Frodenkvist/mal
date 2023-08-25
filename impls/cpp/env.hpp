#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>
#include <vector>

using std::string;
using std::map;
using std::shared_ptr;
using std::vector;

class EnvData;

typedef shared_ptr<EnvData> Env;

class EnvData
{
  map<string, MalType> data_;
  Env outer_;

public:
  EnvData(Env outer = nullptr, const vector<MalType>& binds = {}, const vector<MalType>& exprs = {});

  MalType set(const string& key, MalType operation);
  MalType get(const string& key);

private:
  EnvData* find(const string& key);
};
