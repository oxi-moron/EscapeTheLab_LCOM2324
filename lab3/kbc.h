#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

int (kbd_issue_command_arg) (uint8_t arg);
int (kbd_issue_command) (uint8_t cmd);

#endif
