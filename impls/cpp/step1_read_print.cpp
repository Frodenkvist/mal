#include <string>
#include <iostream>
#include "linenoise.hpp"
#include "reader.hpp"
#include "printer.hpp"
#include "types.hpp"
#include "error.hpp"

using std::string;
using std::cout;
using std::getline;
using std::endl;
using std::cin;

MalType READ(string input)
{
  return Tokenizer::readStr(input);
}

MalType EVAL(MalType input)
{
  return input;
}

string PRINT(MalType input)
{
  return Printer::prStr(input, true);
}

string rep(string input)
{
  auto ast = READ(input);
  auto result = EVAL(ast);
  auto output = PRINT(result);

  return output;
}

int main()
{
  const auto history_path = "history.txt";
  linenoise::LoadHistory(history_path);

  string input;

  while(true)
  {
    auto quit = linenoise::Readline("user> ", input);
    if(quit) break;

    try
    {
      cout << rep(input) << endl;
    }
    catch(EOFException& err)
    {
      err.log();
    }
    catch(InvalidKeyException& err)
    {
      err.log();
    }

    linenoise::AddHistory(input.c_str());
  }

  linenoise::SaveHistory(history_path);

  return 0;
}

