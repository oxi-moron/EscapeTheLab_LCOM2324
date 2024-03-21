//
// Created by ricardo on 3/14/24.
//

#ifndef G6_MOUSE_H
#define G6_MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"

int (mouse_subscribe_int) (uint8_t* bit_no);
int (mouse_unsubscribe_int) ();
void (mouse_ih) ();
int (mouse_disable_data_reporting) ();
int (my_mouse_enable_data_reporting) ();

#endif //G6_MOUSE_H
