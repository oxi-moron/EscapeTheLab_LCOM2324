#include "kbc.h"

int kbd_hook_id = KBD_BIT_NO;
uint8_t scancode;
uint32_t cnt = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(kbd_hook_id);
    return sys_irqsetpolicy(KBD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&kbd_hook_id);
}

void (kbc_ih)() {
    cnt++;
    util_sys_inb(KBD_OUT_BUF, &scancode);
}

int (kbc_enable_interrupts)() {
    uint8_t cb;
    sys_outb(KBD_CMD_REG, READ_CMD_BYTE);
    util_sys_inb(KBD_ARG_REG, &cb);
    cb |= KBD_ENABLE_INT;
    sys_outb(KBD_CMD_REG, WRITE_CMD_BYTE);
    return sys_outb(KBD_ARG_REG, cb);
}
