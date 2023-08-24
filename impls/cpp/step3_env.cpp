#include <string>
#include <iostream>
#include <vector>
#include "linenoise.hpp"
#include "reader.hpp"
#include "printer.hpp"
#include "types.hpp"
#include "env.hpp"
#include "error.hpp"

using std::string;
using std::cout;
using std::getline;
using std::endl;
using std::cin;
using std::function;
using std::vector;

Env env = Env(new EnvData());

MalType EVAL(const MalType& input, Env& env);

MalType READ(const string& input)
{
  return Tokenizer::readStr(input);
}

MalType evalAst(const MalType& ast, Env& env)
{
  if(typeid(*ast) == typeid(MalSymbol))
  {
    auto* symbol = dynamic_cast<MalSymbol*>(&*ast);

    return env->get(symbol->getSymbol());
  }

  if(typeid(*ast) == typeid(MalList))
  {
    vector<MalType> elements;

    auto* malList = dynamic_cast<MalList*>(&*ast);

    for(auto mal : *malList)
    {
      elements.push_back(EVAL(mal, env));
    }

    return MalType(new MalList(elements));
  }

  if(typeid(*ast) == typeid(MalVector))
  {
    vector<MalType> elements;

    auto* malVector= dynamic_cast<MalVector*>(&*ast);

    for(auto mal : *malVector)
    {
      elements.push_back(EVAL(mal, env));
    }

    return MalType(new MalVector(elements));
  }

  if(typeid(*ast) == typeid(MalHashMap))
  {
    vector<MalType> elements;

    auto* malHashMap = dynamic_cast<MalHashMap*>(&*ast);

    for(auto pair : *malHashMap)
    {
      elements.push_back(READ(pair.first));
      elements.push_back(EVAL(pair.second, env));
    }

    return MalType(new MalHashMap(elements));
  }

  return ast;
}

MalType EVAL(const MalType& input, Env& env)
{
  if(typeid(*input) != typeid(MalList))
  {
    return evalAst(input, env);
  }

  auto* malList = dynamic_cast<MalList*>(&*input);

  if(malList->isEmpty())
  {
    return input;
  }

  auto symbol = dynamic_cast<MalSymbol*>(&*(*malList)[0]);

  if(symbol->getSymbol() == "def!")
  {
    auto key = dynamic_cast<MalSymbol*>(&*(*malList)[1]);
    return env->set(key->getSymbol(), EVAL((*malList)[2], env));
  }

  if(symbol->getSymbol() == "let*")
  {
    auto newEnv = Env(new EnvData(env));
    auto* defs = dynamic_cast<MalEnumerable*>(&*(*malList)[1]);

    for(auto itr = defs->begin(); itr != defs->end(); ++itr)
    {
      auto* key = dynamic_cast<MalSymbol*>(&*(*itr));
      ++itr;
      auto value = EVAL(*itr, newEnv);

      newEnv->set(key->getSymbol(), value);
    }

    return EVAL((*malList)[2], newEnv);
  }

  auto evalList = evalAst(input, env);
  auto* listPtr = dynamic_cast<MalList*>(&*evalList);

  auto* operation = dynamic_cast<MalOperation*>(&*(*listPtr)[0]);

  vector<MalType> args;

  for(auto itr = listPtr->begin() + 1; itr != listPtr->end(); ++itr)
  {
    args.push_back(*itr);
  }

  return operation->apply(args);
}

string PRINT(const MalType& input)
{
  return Printer::prStr(input);
}

string rep(const string& input)
{
  auto ast = READ(input);
  auto result = EVAL(ast, env);
  auto output = PRINT(result);

  return output;
}

int main()
{
  env->set("+", std::make_shared<MalAddOperation>());
  env->set("-", std::make_shared<MalSubOperation>());
  env->set("*", std::make_shared<MalMultOperation>());
  env->set("/", std::make_shared<MalDivOperation>());

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
    catch(InvalidSymbolException& err)
    {
      err.log();
    }

    linenoise::AddHistory(input.c_str());
  }

  linenoise::SaveHistory(history_path);

  return 0;
}

