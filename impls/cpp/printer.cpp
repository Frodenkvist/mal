#include "printer.hpp"

#include "types.hpp"

string Printer::prStr(MalType malType)
{
  return malType->getString();
}

