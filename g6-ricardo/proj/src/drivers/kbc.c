#include "kbc.h"

int hook_id3 = 1;
uint8_t scancode;
uint32_t cnt = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(hook_id3);
    return sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id3);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_id3);
}

void (kbc_ih)() {
    cnt++;
    util_sys_inb(0x60, &scancode);
}

int (kbc_enable_interrupts)() {
    uint8_t cb;
    sys_outb(0x64, 0x20);
    util_sys_inb(0x60, &cb);
    cb |= 0x01;
    sys_outb(0x64, 0x60);
    return sys_outb(0x60, cb);
}
