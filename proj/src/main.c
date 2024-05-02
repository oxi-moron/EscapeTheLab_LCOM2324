#include <lcom/lcf.h>

#include "drivers/video.h"
#include "drivers/VBE.h"
#include "graphics.h"
#include "drivers/timer.h"
#include "drivers/i8254_timer.h"
#include "game/player.h"
#include "drivers/rtc.h"
#include "drivers/rtc_macros.h"
#include "game/map.h"
#include "game/game_state.h"

extern uint32_t counter;
extern bool game_in_progress;

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

int driver_setup() {
    uint8_t timer_bit_no = TIMER_BIT_NO, rtc_bit_no = RTC_BIT_NO;

    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_subscribe_int(&rtc_bit_no) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_set_alarm() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (timer_set_frequency(0, 20) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (vg_start(DIR_MODE_800X600) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int driver_cleanup() {
    if (rtc_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (timer_unsubscribe_int() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (vg_exit() != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    vg_free_buffer();

    return 0;
}

int (proj_main_loop) (int argc, char *argv[]) {

    struct point2D position = {300, 300};
    player_construct(position, 0);

    if (driver_setup() != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    if (graphics_construct() != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    int ipc_status, r;
    message msg;

    uint32_t timer_irq_set = BIT(TIMER_BIT_NO), rtc_irq_set = BIT(RTC_BIT_NO);

    while(game_in_progress) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d\n", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        timer_ih();
                        if (counter % (sys_hz() / FRAME_RATE) == 0) {
                            if (state_process() != 0) {
                                printf("ERROR: %s\n", __func__ );
                                vg_exit();
                                return 1;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & rtc_irq_set) {
                        rtc_ih();
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    if (driver_cleanup() != 0) {
        printf("ERROR: %s\n", __func__);
        vg_exit();
        return 1;
    }

    return 0;
}
