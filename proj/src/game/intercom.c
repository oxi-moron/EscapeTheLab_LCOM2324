#include "intercom.h"

int (intercom_send_letter) (uint8_t player_no, uint8_t scancode) {
    if (player_no == 1) {
        if (uart_send_data(COM1, scancode) != 0) {
            return 1;
        }
        printf("\n%d: sent letter %x", player_no, scancode);

        while (word[current_index] != 0x17 && scancode == 0x17) {
            if (uart_receive_data(COM2, &word[current_index]) != 0) {
                return 1;
            }
        }
        printf("\n%d: received letter %x", player_no, word[current_index]);
        current_index = (current_index + 1) % 4;
    } else {
        while (word[current_index] != 0x17 && scancode == 0x17) {
            if (uart_receive_data(COM1, &word[current_index]) != 0) {
                return 1;
            }
        }
        printf("\n%d: received letter %x", player_no, word[current_index]);
        current_index = (current_index + 1) % 4;

        if (uart_send_data(COM2, scancode) != 0) {
            return 1;
        }
        printf("\n%d: sent letter %x", player_no, scancode);
    }

    return 0;
}

void (intercom_get_letters) (uint8_t* letters) {
    letters = word;
}
