//
// Created by ricardo on 3/14/24.
//

#include <lcom/lcf.h>
#include "mouse.h"
#include "kbc.h"
#include <assert.h>

int mouse_hook_id;
struct packet pp;
uint8_t byte_no = 0;
bool packet_ready = false;

int (mouse_subscribe_int) (uint8_t* bit_no) {

    mouse_hook_id = *bit_no;

    assert(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) == 0);

    return 0;
}

int (mouse_unsubscribe_int) () {

    assert(sys_irqrmpolicy(&mouse_hook_id) == 0);

    return 0;
}

void (mouse_ih) () {

    uint8_t data;
    assert(kbd_read_data(&data) == 0);

    switch(byte_no) {
        case 0:
            if ((data & CHECK_BIT)) {
                pp.bytes[0] = data;
                pp.y_ov = YOV(data);
                pp.x_ov = XOV(data);
                pp.delta_x = MSB_X_DELTA(data);
                pp.delta_y = MSB_Y_DELTA(data);
                pp.lb = LB(data);
                pp.rb = RB(data);
                pp.mb = MB(data);
                byte_no = 1;
            }
            else {
                byte_no = 0;
            }
            break;
        case 1:
            pp.bytes[1] = data;
            pp.delta_x = (pp.delta_x == 0) ? 0x0000 : 0xFF00;
            pp.delta_x |= data;
            byte_no = 2;
            break;
        case 2:
            pp.bytes[2] = data;
            pp.delta_y = (pp.delta_y == 0) ? 0x0000 : 0xFF00;
            pp.delta_y |= data;
            packet_ready = true;
            byte_no = 0;
    }
}

int (my_mouse_enable_data_reporting) () {
    uint8_t ack;
    do {
        assert(kbd_issue_command(WRITE_TO_MOUSE, KBD_CMD_REG) == 0);
        assert(kbd_issue_command_arg(ENABLE_DATA) == 0);
        assert(kbd_read_data(&ack) == 0);
    } while (ack != ACK_BYTE);

    return 0;
}

int (mouse_disable_data_reporting) () {
    uint8_t ack;
    do {
        assert(kbd_issue_command(WRITE_TO_MOUSE, KBD_CMD_REG) == 0);
        assert(kbd_issue_command_arg(DISABLE_DATA) == 0);
        assert(kbd_read_data(&ack) == 0);
    } while (ack != ACK_BYTE);

    return 0;
}
