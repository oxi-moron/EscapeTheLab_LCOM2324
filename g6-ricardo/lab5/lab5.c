// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

// Any header files included below this line should have been created by you
#include "timer.h"
#include "video.h"
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "VBE.h"

extern uint32_t counter;
extern uint8_t codes[2];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g6/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g6/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

   if (vg_set_mode(mode) != 0) {
       printf("ERROR: %s", __func__ );
       return 1;
   }

  sleep(delay);

  if(vg_exit() != 0) {
      printf("ERROR: %s", __func__ );
      return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

    if (vg_map_vram(mode) != 0) {
      printf("ERROR: %s", __func__ );
      return 1;
  }

  if (vg_set_mode(mode) != 0) {
      printf("ERROR: %s", __func__ );
      return 1;
  }

  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
      printf("ERROR: %s", __func__ );
      return 1;
  }

    int ipc_status, r;
    message msg;

    uint8_t bit_no = KBD_BIT_NO;

    assert(subscribe_kbd_interrupts(&bit_no) == 0);

    uint32_t irq_set = BIT(bit_no);
    uint8_t cw;

    while(codes[0] != ESC_BREAK_CODE) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            kbc_ih();
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    assert(unsubscribe_kbd_interrupts() == 0);


    if(vg_exit() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

    if (vg_map_vram(mode) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (vg_set_mode(mode) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    uint16_t width, height;

    if (vg_get_rectangle_dimensions(no_rectangles, &width, &height) != 0) {
        printf("ERROR: %s", __func__);
        return 1;
    }

    for (int row = 0; row < no_rectangles; row++) {
        for (int column = 0; column < no_rectangles; column++) {
            uint32_t color;
            if (mode == INDEXED_MODE) {
                if (vg_get_indexed_color(row, column, first, step, no_rectangles, &color) != 0) {
                    printf("ERROR: %s", __func__);
                    return 1;
                }
            } else {
                if (vg_get_direct_color(row, column, first, step, &color) != 0) {
                    printf("ERROR: %s", __func__);
                    return 1;
                }
            }

            if(vg_draw_rectangle(column * width, row * height, width, height, color) != 0) {
                printf("ERROR: %s", __func__);
                return 1;
            }
        }
    }

    int ipc_status, r;
    message msg;

    uint8_t bit_no = KBD_BIT_NO;

    assert(subscribe_kbd_interrupts(&bit_no) == 0);

    uint32_t irq_set = BIT(bit_no);
    uint8_t cw;

    while(codes[0] != ESC_BREAK_CODE) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            kbc_ih();
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    assert(unsubscribe_kbd_interrupts() == 0);


    if(vg_exit() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    if (vg_map_vram(INDEXED_MODE) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (vg_set_mode(INDEXED_MODE) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    xpm_image_t img;
    uint8_t *map;

    map = xpm_load(xpm, XPM_INDEXED, &img);

    if (vg_draw_xpm(x, y, img.width, img.size, map) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    int ipc_status, r;
    message msg;

    uint8_t bit_no = KBD_BIT_NO;

    if(subscribe_kbd_interrupts(&bit_no) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    uint32_t irq_set = BIT(bit_no);
    uint8_t cw;

    while(codes[0] != ESC_BREAK_CODE) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            kbc_ih();
                        }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    if (unsubscribe_kbd_interrupts() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }


    if(vg_exit() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

    if (xi != xf && yi != yf) return 1;

    if (vg_map_vram(INDEXED_MODE) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (vg_set_mode(INDEXED_MODE) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    xpm_image_t img;
    uint8_t *map;

    map = xpm_load(xpm, XPM_INDEXED, &img);

    int ipc_status, r;
    message msg;

    uint8_t kbd_bit_no = KBD_BIT_NO;
    uint8_t timer_bit_no = TIMER_BIT_NO;

    if(subscribe_kbd_interrupts(&kbd_bit_no) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (timer_subscribe_int(&timer_bit_no) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    uint32_t kbd_irq_set = BIT(kbd_bit_no);
    uint32_t timer_irq_set = BIT(timer_bit_no);
    uint8_t cw; uint32_t freq;

    uint16_t x = xi, y = yi, cooldown = 0;

    if (timer_get_freq(&freq) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (vg_draw_xpm(x, y, img.width, img.size, map) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (speed < 0) cooldown = abs(speed);

    while(codes[0] != ESC_BREAK_CODE) {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & kbd_irq_set) {
                        assert(util_sys_inb(KBD_STATUS_REG, &cw) == 0);
                        if (cw & KBD_OBF) {
                            kbc_ih();
                        }
                    }
                    if (msg.m_notify.interrupts & timer_irq_set) {
                            timer_int_handler();
                            if (counter == freq / fr_rate) {
                                if (x != xf || y != yf) {
                                    if (vg_draw_rectangle(x, y, img.width, img.height, 0) != 0) {
                                        printf("ERROR: %s", __func__);
                                        return 1;
                                    }
                                    if (speed < 0) {
                                        cooldown--;
                                        if (cooldown == 0) {
                                            x += 1; y += 1;
                                            cooldown = abs(speed);
                                        }
                                    } else {
                                        x += speed; y += speed;
                                    }
                                    if (x > xf) x = xf; if (y > yf) y = yf;
                                }
                                if (vg_draw_xpm(x, y, img.width, img.size, map) != 0) {
                                    printf("ERROR: %s", __func__);
                                    return 1;
                                }
                                counter = 0;
                            }
                    }
                    break;
                default:
                    break;
            }
        } else {
        }
    }

    if (timer_unsubscribe_int() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (unsubscribe_kbd_interrupts() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }


    if(vg_exit() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
