#ifndef SRC_CMDFUNCS_H
#define SRC_CMDFUNCS_H

#include "ast.h"
#include <inttypes.h>
#include <Zydis/Zydis.h>    // zydis
#include <assert.h>
#include <bfd.h>            // bfd

asymbol **symbol_table;
long num_symbols;

void bfd_func(char *file);

// A tedious but simple parsing function, parsing
// user input string into a command data structure.
 Cmd_t
parseCommand (char *s, int pid);

// Given a command and execute it.
 void
execCommand (Cmd_t c);

 void
loopCommand (int pid);

#endif //SRC_CMDFUNCS_H
