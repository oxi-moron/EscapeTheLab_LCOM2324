#include <lcom/lcf.h>

#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

int hook_id = 0;
uint8_t codes[2];

int (subscribe_kbd_interrupts) (uint8_t *bit_no) {

  hook_id = *bit_no;

  assert(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) == 0);

  return 0;

}

int (unsubscribe_kbd_interrupts) () {

  assert(sys_irqrmpolicy(&hook_id) == 0);

  return 0;

}

void (kbc_ih) () {

  if (codes[0] == TWO_PART_CODE)
    assert(util_sys_inb(KBD_OUT_BUF, &codes[1]) == 0);
  else
    assert(util_sys_inb(KBD_OUT_BUF, &codes[0]) == 0);
}
