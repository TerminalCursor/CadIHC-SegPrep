#ifndef INC_LIB_MONAD_H
#define INC_LIB_MONAD_H

#include <lib/bool.h>

struct Result {
  void* data;
  bool_t success;
};

#endif
