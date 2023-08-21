#include <string>
#include <iostream>
#include "linenoise.hpp"

using std::string;
using std::cout;
using std::getline;
using std::endl;
using std::cin;

string READ(string input)
{
  return input;
}

string EVAL(string input)
{
  return input;
}

string PRINT(string input)
{
  return input;
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

    cout << rep(input) << endl;

    linenoise::AddHistory(input.c_str());
  }

  linenoise::SaveHistory(history_path);

  return 0;
}

