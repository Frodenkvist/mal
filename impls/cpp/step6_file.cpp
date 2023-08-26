#include <string>
#include <iostream>
#include <vector>
#include "linenoise.hpp"
#include "reader.hpp"
#include "printer.hpp"
#include "types.hpp"
#include "env.hpp"
#include "error.hpp"
#include "core.hpp"

using std::string;
using std::cout;
using std::getline;
using std::endl;
using std::cin;
using std::function;
using std::vector;

Env replEnv = Env(new EnvData());

MalType EVAL(MalType input, Env env);

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

MalType EVAL(MalType input, Env env)
{
  if(env == nullptr) env = replEnv;

  while(true)
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

    if(typeid(*(*malList)[0]) == typeid(MalSymbol))
    {
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

        env = newEnv;
        input = (*malList)[2];

        continue;
      }

      if(symbol->getSymbol() == "do")
      {
        vector<MalType> elements;
        for(auto itr = malList->begin() + 1; itr != malList->end() - 1; ++itr)
        {
          elements.push_back(*itr);
        }

        evalAst(MalType(new MalList(elements)), env);

        input = *(malList->end() - 1);
        continue;
      }

      if(symbol->getSymbol() == "if")
      {
        auto result = EVAL((*malList)[1], env);

        if(result != MNil && result != MFalse)
        {
          input = (*malList)[2];
          continue;
        }

        if(malList->size() == 3)
        {
          return MNil;
        }

        input = (*malList)[3];
        continue;
      }

      if(symbol->getSymbol() == "fn*")
      {
        vector<MalType> bindings;
        auto* argsList = dynamic_cast<MalEnumerable*>(&*(*malList)[1]);

        for(auto itr = argsList->begin(); itr != argsList->end(); ++itr)
        {
          bindings.push_back(*itr);
        }

        return MalType(new MalFunction(bindings, (*malList)[2], env));
      }
    }

    auto evalList = evalAst(input, env);
    auto* listPtr = dynamic_cast<MalList*>(&*evalList);

    vector<MalType> args;

    for(auto itr = listPtr->begin() + 1; itr != listPtr->end(); ++itr)
    {
      args.push_back(*itr);
    }

    auto* operation = dynamic_cast<MalOperation*>(&*(*listPtr)[0]);

    if(const auto* function = dynamic_cast<MalFunction*>(operation))
    {
      input = function->getBody();
      env = function->makeEnv(args);
      continue;
    }

    return operation->apply(args);
  }
}

string PRINT(const MalType& input)
{
  return Printer::prStr(input, true);
}

string rep(const string& input)
{
  auto ast = READ(input);
  auto result = EVAL(ast, replEnv);
  auto output = PRINT(result);

  return output;
}

int main(int argc, char *argv[])
{
  for(auto itr = Core::ns.begin(); itr != Core::ns.end(); ++itr)
  {
    replEnv->set(itr->first, itr->second);
  }

  rep("(def! not (fn* (a) (if a false true)))");
  rep("(def! load-file (fn* (f) (eval (read-string (str \"(do \" (slurp f) \"\nnil)\")))))");

  if(argc > 1)
  {
    vector<MalType> elements;

    for(int i = 2; i < argc; ++i)
    {
      elements.push_back(MalType(new MalString(string(argv[i]))));
    }

    replEnv->set("*ARGV*", MalType(new MalList(elements)));

    rep("(load-file \"" + string(argv[1]) + "\")");
    
    return 0;
  }

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

