#pragma once

#include "types.hpp"
#include <string>

using std::string;

class Printer
{
public:
  static string prStr(const MalType& malType, const bool& printReadably = false);
};


