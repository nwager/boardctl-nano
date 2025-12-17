#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

// Command code definitions
/* __STARTDEF__ command_code */
#define CMD_RESET      0
#define CMD_RECOVERY   1
#define CMD_POWER      2
#define CMD_START      255
/* __ENDDEF__ command_code */

typedef uint8_t command_code_t;

struct command {
  command_code_t code;
};

#endif // COMMAND_H
