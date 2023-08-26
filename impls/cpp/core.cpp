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
  { "println", MalType(new MalPrintlnOperation()) },
  { "read-string", MalType(new MalReadStringOperation()) },
  { "slurp", MalType(new MalSlurpOperation()) },
  { "eval", MalType(new MalEvalOperation()) },
  { "atom", MalType(new MalAtomOperation()) },
  { "atom?", MalType(new MalIsAtomOperation()) },
  { "deref", MalType(new MalDerefOperation()) },
  { "reset!", MalType(new MalResetOperation()) },
  { "swap!", MalType(new MalSwapOperation()) },
  { "*ARGV*", MalType(new MalList({})) },
  { "cons", MalType(new MalConsOperation()) },
  { "concat", MalType(new MalConcatOperation()) },
  { "vec", MalType(new MalVecOperation()) }
};
