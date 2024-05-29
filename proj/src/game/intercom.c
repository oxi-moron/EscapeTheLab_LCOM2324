#include "intercom.h"

extern int player_no;

int (intercom_send_letter) (uint8_t scancode) {
    if (player_no == 1) {
        if (uart_send_data(COM1, scancode) != 0) {
            return 1;
        }
        printf("sent letter\n");

        if (uart_receive_data(COM2, &word[current_index]) != 0) {
            return 1;
        } else current_index = (current_index + 1) % 4;
        printf("received letter\n");
    } else {
        if (uart_receive_data(COM1, &word[current_index]) != 0) {
            return 1;
        } else current_index = (current_index + 1) % 4;
        printf("received letter\n");

        if (uart_send_data(COM2, scancode) != 0) {
            return 1;
        }
        printf("sent letter\n");
    }

    return 0;
}
