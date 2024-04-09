#include <lcom/lcf.h>
#include "timer.h"
#include "i8254_timer.h"

int timer_hook_id;
uint32_t counter = 0;

int (timer_get_frequency)(uint32_t* freq) {
    *freq = sys_hz();
    return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    // TODO: implement
    return 0;
}

int (timer_subscribe_int)(uint8_t* bit_no) {
    timer_hook_id = *bit_no;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&timer_hook_id) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

void (timer_ih) () {
    counter++;
}

int timer_write_cmd(uint8_t cmd) {
    if (sys_outb(TIMER_CTRL, cmd) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int timer_read_status(uint8_t* st, uint8_t timer) {
    if (util_sys_inb(TIMER_0 + timer, st) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}
// More could be needed
