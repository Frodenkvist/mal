#pragma once

#include "types.hpp"

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class MalException : public std::exception
{
protected:
  string msg_;

public:
  MalException(string msg): msg_(msg) {}

  virtual void log() = 0;

  string getMsg() { return msg_; }
};

class EOFException : public MalException
{
public:
  EOFException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "EOF Exception occurred: " << msg_ << endl;
  }
};

class InvalidKeyException : public MalException
{
public:
  InvalidKeyException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Invalid Key Exception: " << msg_ << endl;
  }
};

class InvalidSymbolException : public MalException
{
public:
  InvalidSymbolException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Invalid Symbol Exception: " << msg_ << endl;
  }
};

class IndexOutOfBoundsException : public MalException
{
public:
  IndexOutOfBoundsException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Index Out Of Bounds Exception: " << msg_ << endl;
  }
};

class MalTypeException : public std::exception
{
  MalType mal_;

public:
  MalTypeException(const MalType& mal): mal_(mal) {};

  MalType getMal() { return mal_; }
};
