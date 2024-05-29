#ifndef G6_INTERCOM_H
#define G6_INTERCOM_H

#include <lcom/lcf.h>

#include "../drivers/serial_port.h"
#include "../drivers/uart_macros.h"

static uint8_t word[4];
static int current_index = 0;

int (intercom_send_letter) (uint8_t scancode);

#endif //G6_INTERCOM_H
