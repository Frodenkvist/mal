#pragma once

#define _ASSERT(file, line, condition, ...) \
  if(!(condition)) { \
    printf("Assertion failed at %s(%d): ", file, line); \
    printf(__VA_ARGS__); \
    exit(1); \
  }

#define ASSERT(condition, ...) \
  _ASSERT(__FILE__, __LINE__, condition, __VA_ARGS__)
