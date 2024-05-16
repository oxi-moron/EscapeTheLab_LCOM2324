#include <lcom/lcf.h>
#include "timer.h"

#include "i8254.h"

uint32_t counter = 0;
int timer_hook_id;

int (timer_subscribe_int)(uint8_t *bit_no) {

    timer_hook_id = *bit_no;

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
        printf("ERROR: sys_irqsetpolicy\n");
        return 1;
    }

    return 0;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&timer_hook_id) != 0) {
        printf("ERROR: sys_irqrmpolicy\n");
        return 1;
    }

    return 0;
}

void (timer_int_handler)() {
    counter++;
}

int (timer_get_freq)(uint32_t* freq) {
    *freq = sys_hz();

    return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    uint8_t cw;

    if (timer_get_conf(timer, &cw) != 0) {
        printf("ERROR: timer_get_conf\n");
        return 1;
    }

    cw &= 0x0F;

    switch (timer) {
        case 0:
            cw |= TIMER_SEL0;
            break;
        case 1:
            cw |= TIMER_SEL1;
            break;
        case 2:
            cw |= TIMER_SEL2;
            break;
    }

    cw |= TIMER_LSB_MSB;

    if (sys_outb(TIMER_CTRL, cw) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    if (freq < 20 || freq > TIMER_FREQ) return 1;

    uint16_t div = TIMER_FREQ / freq;
    uint8_t lsb;
    uint8_t msb;

    if (util_get_LSB(div, &lsb) != 0) {
        printf("ERROR: util_get_LSB\n");
        return 1;
    }

    if (util_get_MSB(div, &msb) != 0) {
        printf("ERROR: util_get_msb\n");
        return 1;
    }

    if (sys_outb(TIMER_0 + timer, lsb) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    if (sys_outb(TIMER_0 + timer, msb) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    uint8_t cw = (TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_);

    if (sys_outb(TIMER_CTRL, cw) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    if (util_sys_inb(TIMER_0 + timer, st) != 0) {
        printf("ERROR: sys_inb\n");
        return 1;
    }

    return 0;
}
