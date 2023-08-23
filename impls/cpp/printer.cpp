#include "printer.hpp"

#include "types.hpp"

string Printer::prStr(MalType malType, bool printReadably)
{
  return malType->getString(printReadably);
}

