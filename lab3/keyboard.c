#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "keyboard.h"
#include "kbc.h"
#include "i8042.h"

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

int (kbd_read_data) (uint8_t *data) {

    uint8_t st, attempts = MAX_ATTEMPTS;
    while(attempts) {
        util_sys_inb(KBD_STATUS_REG, &st);
        if(st & KBD_OBF) {
            util_sys_inb(KBD_OUT_BUF, data);
            if ((st & (KBD_PAR_ERR | KBD_TO_ERR | KBD_AUX)) == 0)
                return 0;
            else
                return 1;
        }
        attempts--;
        tickdelay(micros_to_ticks(WAIT_KBD));
    }

    return 1;
}

int (reset_keyboard_int) () {

    uint8_t cb;

    assert(kbd_issue_command(READ_CMD_BYTE) == 0);
    assert(kbd_read_data(&cb) == 0);

    cb |= KBD_ENABLE_INT;

    assert(kbd_issue_command(WRITE_CMD_BYTE) == 0);
    assert(kbd_issue_command_arg(cb) == 0);

    return 0;
}
