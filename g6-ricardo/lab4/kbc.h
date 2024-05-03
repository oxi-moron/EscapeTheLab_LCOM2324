#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

int (kbd_issue_command_arg) (uint8_t arg);
int (kbd_issue_command) (uint8_t cmd, uint8_t port);
int (kbd_read_data) (uint8_t *data);

#endif
