#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1
#define KBD_HOOK_ID 1

#define ESC_BREAK_CODE 0x81
#define TWO_PART_CODE 0xE0

#define KBD_OUT_BUF 0x60
#define KBD_STATUS_REG 0x64
#define KBD_CMD_REG 0x64

#define KBD_OBF 0x01

#define MAKE_BREAK_BIT 0x80

#endif
