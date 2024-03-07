//
// Created by ricardo on 3/7/24.
//

#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

int (subscribe_kbd_interrupts) (uint8_t *bit_no);
int (unsubscribe_kbd_interrupts) ();
void (kbc_ih) ();

#endif // LAB3_KEYBOARD_H
