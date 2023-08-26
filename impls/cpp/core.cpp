#include "core.hpp"

#include <memory>

map<string, MalType> Core::ns = {
  { "prn", MalType(new MalPrnOperation()) },
  { "list", MalType(new MalListOperation()) },
  { "list?", MalType(new MalIsListOperation()) },
  { "empty?", MalType(new MalIsEmptyOperation()) },
  { "count", MalType(new MalCountOperation()) },
  { "=", MalType(new MalEqualsOperation()) },
  { "<", MalType(new MalLTOperation()) },
  { "<=", MalType(new MalLTEOperation()) },
  { ">", MalType(new MalGTOperation()) },
  { ">=", MalType(new MalGTEOperation()) },
  { "+", MalType(new MalAddOperation()) },
  { "-", MalType(new MalSubOperation()) },
  { "*", MalType(new MalMultOperation()) },
  { "/", MalType(new MalDivOperation()) },
  { "pr-str", MalType(new MalPrStrOperation()) },
  { "str", MalType(new MalStrOperation()) },
  { "println", MalType(new MalPrintlnOperation()) }
};
