#pragma once

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class EOFException : std::exception
{
  string msg_;

public:
  EOFException(string msg): msg_(msg) {}

  void log()
  {
    cout << "EOF Exception occurred: " << msg_ << endl;
  }
};

