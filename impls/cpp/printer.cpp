#include "printer.hpp"

#include "types.hpp"

string Printer::prStr(const MalType& malType, const bool& printReadably)
{
  return malType->getString(printReadably);
}

