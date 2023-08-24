#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>

using std::string;
using std::map;
using std::shared_ptr;

class EnvData;

typedef shared_ptr<EnvData> Env;

class EnvData
{
  map<string, MalType> data_;
  Env outer_;

public:
  EnvData(Env outer = nullptr);

  MalType set(const string& key, MalType operation);
  MalType get(const string& key);

private:
  EnvData* find(const string& key);
};
