#pragma once

#include "types.hpp"

#include <string>
#include <map>
#include <memory>

using std::string;
using std::map;
using std::shared_ptr;

struct Env
{
  map<string, shared_ptr<MalOperation>> operations;

  Env(map<string, shared_ptr<MalOperation>> inOperations): operations(inOperations) {}

  static Env globalEnv;
};

Env Env::globalEnv = Env({
  { "+", std::make_shared<MalAddOperation>()},
  { "-", std::make_shared<MalSubOperation>()},
  { "*", std::make_shared<MalMultOperation>()},
  { "/", std::make_shared<MalDivOperation>()}
});

