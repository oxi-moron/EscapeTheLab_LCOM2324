//
// Created by ricardo on 3/7/24.
//

#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

#include <lcom/lcf.h>

int (subscribe_kbd_interrupts) (uint8_t *bit_no);
int (unsubscribe_kbd_interrupts) ();
void (kbc_ih) ();
int (kbd_issue_command) (uint8_t cmd);
int (kbd_issue_command_arg) (uint8_t arg);
int (kbd_read_data) (uint8_t *data);
int (reset_keyboard_int) ();

#endif // LAB3_KEYBOARD_H
