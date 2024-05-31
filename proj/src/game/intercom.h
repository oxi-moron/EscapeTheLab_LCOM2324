#ifndef G6_INTERCOM_H
#define G6_INTERCOM_H

#include <lcom/lcf.h>

#include "../drivers/serial_port.h"
#include "../drivers/uart_macros.h"


static uint8_t word[4] = {0, 0, 0, 0};
static uint8_t password[4] = {0x26, 0x2E, 0x18, 0x32};
static int current_index = 0;

int (intercom_send_letter) (uint8_t player_no, uint8_t scancode);
void (intercom_get_letters) (uint8_t* letters);
void (intercom_get_password) (uint8_t* pw);
bool intercom_password_entered();
void (intercom_reset) ();

#endif //G6_INTERCOM_H
