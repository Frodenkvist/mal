#pragma once

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class MalException : std::exception
{
protected:
  string msg_;

public:
  MalException(string msg): msg_(msg) {}

  virtual void log() = 0;
};

class EOFException : MalException
{
public:
  EOFException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "EOF Exception occurred: " << msg_ << endl;
  }
};

class InvalidKeyException : MalException
{
public:
  InvalidKeyException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Invalid Key Exception: " << msg_ << endl;
  }
};

class InvalidSymbolException : MalException
{
public:
  InvalidSymbolException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Invalid Symbol Exception: " << msg_ << endl;
  }
};

class IndexOutOfBoundsException : MalException
{
public:
  IndexOutOfBoundsException(string msg): MalException(msg) {};

  void log() override
  {
    cout << "Index Out Of Bounds Exception: " << msg_ << endl;
  }
};
