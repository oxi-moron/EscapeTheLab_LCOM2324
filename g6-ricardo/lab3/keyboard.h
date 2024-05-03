//
// Created by ricardo on 3/7/24.
//

#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

#include <lcom/lcf.h>

int (subscribe_kbd_interrupts) (uint8_t *bit_no);
int (unsubscribe_kbd_interrupts) ();
void (kbc_ih) ();
int (reset_keyboard_int) ();

#endif // LAB3_KEYBOARD_H
