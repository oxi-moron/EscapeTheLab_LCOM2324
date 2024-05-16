//
// Created by ricardo on 3/14/24.
//

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

uint32_t timer_counter = 0;
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
    timer_counter++;
}
