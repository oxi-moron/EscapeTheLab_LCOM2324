#include <lcom/lcf.h>

#include "video.h"
#include "VBE.h"
#include "graphics.h"
#include "timer.h"
#include "i8254_timer.h"

extern uint32_t counter;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/g6/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/g6/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (proj_main_loop) (int argc, char *argv[]) {

    if (vg_set_mode(DIR_MODE_800X600) != 0) {
        printf("ERROR: %s", __func__);
        return 1;
    }

    if (vg_map_vram(DIR_MODE_800X600) != 0) {
        printf("ERROR: %s", __func__);
        vg_exit();
        return 1;
    }

    int ipc_status, r;
    message msg;

    uint8_t timer_bit_no = TIMER_BIT_NO;

    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("ERROR: %s", __func__);
        vg_exit();
        return 1;
    }

    uint32_t timer_irq_set = BIT(timer_bit_no);

    if (draw_current_frame() != 0) {
        printf("ERROR: %s", __func__ );
        vg_exit();
        return 1;
    };

    while(counter < 180) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    if (timer_unsubscribe_int() != 0) {
        printf("ERROR: %s", __func__);
        vg_exit();
        return 1;
    }

    if (vg_exit() != 0) {
        printf("ERROR: %s", __func__);
        return 1;
    }

    return 0;
}
