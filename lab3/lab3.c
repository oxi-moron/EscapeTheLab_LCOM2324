#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "keyboard.h"
#include "kbc.h"
#include "i8042.h"

extern int hook_id;
extern uint8_t codes[];
extern uint32_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g6/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g6/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {

  int ipc_status, r;
  message msg;

  uint8_t bit_no = KBD_HOOK_ID;
  bool two_part = false;

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
              if (codes[0] != 0xE0) {
                kbd_print_scancode(!((codes[0] & MAKE_BREAK_BIT) >> 7), 1, codes);
              }
              else {
                  if (two_part) {
                    kbd_print_scancode(!((codes[1] & MAKE_BREAK_BIT) >> 7), 2, codes);
                    codes[0] = 0x00;
                }
                two_part = !two_part;
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

  assert(unsubscribe_kbd_interrupts() == 0);
  assert(kbd_print_no_sysinb(counter) == 0);

  return 0;
}

int(kbd_test_poll)() {

    uint8_t data = 0;
    bool two_part = false;

    while (data != ESC_BREAK_CODE) {
        if (kbd_read_data(&data) == 0) {
            codes[0] = data;
            if (codes[0] != 0xE0) {
                kbd_print_scancode(!((codes[0] & MAKE_BREAK_BIT) >> 7), 1, codes);
            }
            else {
              if (two_part) {
                kbd_print_scancode(!((codes[1] & MAKE_BREAK_BIT) >> 7), 2, codes);
                codes[0] = 0x00;
              }
              two_part = !two_part;
            }
        }
    } 

    assert(reset_keyboard_int() == 0);

    assert(kbd_print_no_sysinb(counter) == 0);

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
