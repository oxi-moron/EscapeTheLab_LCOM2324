#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1
#define KBD_BIT_NO 1

#define ESC_BREAK_CODE 0x81
#define TWO_PART_CODE 0xE0

#define KBD_OUT_BUF 0x60
#define KBD_STATUS_REG 0x64
#define KBD_CMD_REG 0x64
#define KBD_ARG_REG 0x60

#define KBD_OBF 0x01
#define KBD_IBF 0x02

#define MAKE_BREAK_BIT 0x80

#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

#define WAIT_KBD 20000

#define KBD_TO_ERR 0x40
#define KBD_PAR_ERR 0x80
#define KBD_AUX 0x20

#define KBD_ENABLE_INT 0x01

#define MAX_ATTEMPTS 10

#endif
