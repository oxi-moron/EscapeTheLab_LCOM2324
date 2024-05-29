#ifndef G6_SERIAL_PORT_H
#define G6_SERIAL_PORT_H

#include <lcom/lcf.h>

int uart_send_data(uint16_t com_address, uint8_t data);
int uart_receive_data(uint16_t com_address, uint8_t* data);
int uart_config(uint16_t com_address);

#endif //G6_SERIAL_PORT_H
