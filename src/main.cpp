#include <Arduino.h>

// Wait this long after setting a pin to allow the connection to settle
#define BUTTON_DELAY_MS 500

#define CMD_START 255
#define CMD_RESET 0
#define CMD_RECOVERY 1
#define CMD_POWER 2

#define PIN_RESET 7
#define PIN_RECOVERY 8
#define PIN_POWER 9

static const int pins[] = {
  PIN_RESET,
  PIN_RECOVERY,
  PIN_POWER
};
#define NUM_PINS (sizeof(pins) / sizeof(pins[0]))

struct command {
  uint8_t state;
};

#define CMD_SIZE (sizeof(CMD_START) + sizeof(struct command))

void connect_pin(int pin, bool state) {
  if (state) {
    // Pull pin LOW
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  } else {
    // "Disconnect" pin by setting to hi-z input
    pinMode(pin, INPUT);
  }
}

void reset_pins() {
  for (size_t i = 0; i < NUM_PINS; i++) {
    connect_pin(pins[i], false);
  }
}

void setup() {
  reset_pins();
  Serial.begin(115200);
}

void wait_for_command(struct command *cmd) {
  size_t i = 0;
  bool cmd_started = false;
  while (i < sizeof(*cmd)) {
    if (!Serial.available())
      continue;

    uint8_t c = Serial.read();

    if (c == CMD_START) {
      i = 0;
      cmd_started = true;
      continue;
    }

    if (!cmd_started)
      continue;

    reinterpret_cast<uint8_t*>(cmd)[i] = c;

    i++;
  }
}

void handle_command(struct command *cmd) {
  reset_pins();
  switch (cmd->state) {
    case CMD_RESET:
      connect_pin(PIN_RESET, true);
      delay(BUTTON_DELAY_MS);
      connect_pin(PIN_RESET, false);
      break;
    case CMD_RECOVERY:
      connect_pin(PIN_RECOVERY, true);
      delay(BUTTON_DELAY_MS);
      connect_pin(PIN_RESET, true);
      delay(BUTTON_DELAY_MS);
      connect_pin(PIN_RESET, false);
      delay(BUTTON_DELAY_MS);
      connect_pin(PIN_RECOVERY, false);
      break;
    case CMD_POWER:
      connect_pin(PIN_POWER, true);
      delay(BUTTON_DELAY_MS);
      connect_pin(PIN_POWER, false);
      break;
    default:
      // Invalid command
      break;
  }
}

void loop() {
  struct command cmd;
  wait_for_command(&cmd);
  handle_command(&cmd);
}
