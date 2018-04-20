#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "macros.h"

void _NO_RET fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("FATAL: ");
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
  exit(EXIT_FAILURE);
}