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

int (kbd_issue_command) (uint8_t cmd) {

    uint8_t st, attempts = 3;
    while(attempts) {
        assert(util_sys_inb(KBD_STATUS_REG, &st) == 0);
        if((st & KBD_IBF) == 0) {
            assert(sys_outb(KBD_CMD_REG, cmd) == 0);
            return 0;
        }
        attempts--;
        tickdelay(WAIT_KBD);
    }

    return 1;
}

int (kbd_issue_command_arg) (uint8_t arg) {

    uint8_t st;
    uint8_t attempts = 3;

    while(attempts) {
        assert(util_sys_inb(KBD_STATUS_REG, &st) == 0);
        if((st & KBD_IBF) == 0) {
            sys_outb(KBD_ARG_REG, arg);
            return 0;
        }
        attempts--;
        tickdelay(WAIT_KBD);
    }

    return 1;
}

int (kbd_read_data) (uint8_t *data) {

    uint8_t st, attempts = 3;
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
        tickdelay(WAIT_KBD);
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
