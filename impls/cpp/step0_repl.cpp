#include <string>
#include <iostream>

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
  string input;

  while(true)
  {
    cout << "user> ";
    if(!getline(cin, input)) break;
    cout << rep(input) << endl;
  }

  return 0;
}

