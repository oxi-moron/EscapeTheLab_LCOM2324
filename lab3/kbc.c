#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "keyboard.h"
#include "kbc.h"
#include "i8042.h"

int (kbd_issue_command) (uint8_t cmd) {

    uint8_t st, attempts = MAX_ATTEMPTS;
    while(attempts) {
        assert(util_sys_inb(KBD_STATUS_REG, &st) == 0);
        if((st & KBD_IBF) == 0) {
            assert(sys_outb(KBD_CMD_REG, cmd) == 0);
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(WAIT_KBD));
    }

    return 1;
}

int (kbd_issue_command_arg) (uint8_t arg) {

    uint8_t st;
    uint8_t attempts = MAX_ATTEMPTS;

    while(attempts) {
        assert(util_sys_inb(KBD_STATUS_REG, &st) == 0);
        if((st & KBD_IBF) == 0) {
            sys_outb(KBD_ARG_REG, arg);
            return 0;
        }
        attempts--;
        tickdelay(micros_to_ticks(WAIT_KBD));
    }

    return 1;
}
