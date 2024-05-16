#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

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
