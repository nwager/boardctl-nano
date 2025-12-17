#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

// Command code definitions
#define CMD_RESET      0
#define CMD_RECOVERY   1
#define CMD_POWER      2
#define CMD_START      0xFF

typedef uint8_t command_code_t;

struct command {
  command_code_t code;
};

#define CMD_SIZE (sizeof(command_code_t) + sizeof(struct command))

#endif // COMMAND_H
