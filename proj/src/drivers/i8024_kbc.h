#ifndef G6_I8024_KBC_H
#define G6_I8024_KBC_H

#include <lcom/lcf.h>

#define KBD_BIT_NO 1
#define KBD_IRQ_LINE 1

#define ESC_BREAK_CODE 0x81

#define MOUSE_BIT_NO 12
#define MOUSE_IRQ_LINE 12

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

#endif //G6_I8024_KBC_H
