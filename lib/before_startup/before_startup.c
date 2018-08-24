// Before startup
//
// This is a function that allows for various C code to be run on start,
// notably after the initial configuration but before main is called

#include <stdio.h>

// the constructor attribute ensures it's called before main
void called_first(void) __attribute__ ((constructor));

void called_first (void) {
  setbuf(stdout, NULL);
}

