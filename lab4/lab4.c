// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
// Any header files included below this line should have been created by you
#include "mouse.h"
#include "timer.h"
#include "i8254.h"

extern bool packet_ready;
extern struct packet pp;
extern uint32_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/g6/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g6/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

    uint8_t mouse_bit_no = MOUSE_BIT_NO;
    assert(mouse_subscribe_int(&mouse_bit_no) == 0);
    assert(my_mouse_enable_data_reporting() == 0);

    int ipc_status, r;
    message msg;

    uint32_t irq_set = BIT(mouse_bit_no);
    uint8_t cw;

    while(cnt > 0) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            mouse_ih();
                            if (packet_ready) {
                                mouse_print_packet(&pp);
                                packet_ready = false;
                                cnt--;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }


    assert(mouse_disable_data_reporting() == 0);
    assert(mouse_unsubscribe_int() == 0);
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {

    uint8_t mouse_bit_no = MOUSE_BIT_NO, timer_bit_no = TIMER_BIT_NO;
    assert(mouse_subscribe_int(&mouse_bit_no) == 0);
    assert(timer_subscribe_int(&timer_bit_no) == 0);
    assert(my_mouse_enable_data_reporting() == 0);

    int ipc_status, r;
    message msg;
    uint32_t timer_irq = BIT(timer_bit_no), mouse_irq = BIT(mouse_bit_no);

    uint32_t freq;
    assert(timer_get_freq(&freq) == 0);
    uint8_t cw;

    while(counter < idle_time * freq) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & mouse_irq) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            mouse_ih();
                            if (packet_ready) {
                                mouse_print_packet(&pp);
                                packet_ready = false;
                                counter = 0;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & timer_irq) {
                        timer_int_handler();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    assert(mouse_disable_data_reporting() == 0);
    assert(timer_unsubscribe_int() == 0);
    assert(mouse_unsubscribe_int() == 0);
    return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
