#include <lcom/lcf.h>
#include "mouse.h"
#include "kbc.h"
#include <assert.h>

int mouse_hook_id;
struct packet pp;
uint8_t byte_no = 0;
bool packet_ready = false;

int line_x = 0, line_y = 0;

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

state_t (change_state) (state_t current_state, event_t current_event, uint8_t x_len) {
    switch (current_state) {
        case INIT:
            if (current_event == L_DOWN) {
                line_x = 0; line_y = 0;
                return DRAW_UP;
            }
            break;
        case DRAW_UP:
            printf("\nline_x: %d ", line_x);
            if (current_event == L_UP && validate_line_up(x_len)) {
                line_x = 0; line_y = 0;
                return VERTEX;
            }
            else if (current_event == ILLEGAL || (current_event == L_UP && !validate_line_up(x_len)))
                return INIT;
            else {
                line_x += pp.delta_x;
                line_y += pp.delta_y;
                return DRAW_UP;
            }
            break;
        case VERTEX:
            if (current_event == R_DOWN)
                return DRAW_DOWN;
            else if (current_event == L_DOWN) {
                line_x = 0; line_y = 0;
                return DRAW_UP;
            }
            else if (current_event == ILLEGAL)
                return INIT;
            else return VERTEX;
            break;
        case DRAW_DOWN:
            printf("\nline_x: %d ", line_x);
            if (current_event == R_UP && validate_line_down(x_len)) {
                return DONE;
            }
            else if (current_event == ILLEGAL || (current_event == R_UP && !validate_line_down(x_len)))
                return INIT;
            else {
                line_x += pp.delta_x;
                line_y += pp.delta_y;
                return DRAW_DOWN;
            }
            break;
        case DONE:
            break;
    }

    return INIT;
}

event_t (evaluate_event) (event_t current_event, uint8_t tolerance) {
    switch (current_event) {
        case NEUTRAL:
            if (pp.mb || abs(pp.delta_x) > tolerance || abs(pp.delta_y) > tolerance || (pp.rb && pp.lb))
                return ILLEGAL;
            else if (pp.rb) return R_DOWN;
            else if (pp.lb) return L_DOWN;
            break;
        case L_DOWN:
            if (pp.mb || (pp.rb && pp.lb) || !pp.lb)
                return ILLEGAL;
            else return MOVE_UP;
            break;
        case MOVE_UP:
            if (pp.mb || pp.rb) return ILLEGAL;
            else if (!pp.lb) return L_UP;
            else return MOVE_UP;
            break;
        case L_UP:
            if (pp.mb || abs(pp.delta_x) > tolerance || abs(pp.delta_y) > tolerance)
                return ILLEGAL;
            else if (pp.rb)
                return R_DOWN;
            else if (pp.lb)
                return L_DOWN;
            else
                return NEUTRAL;
        case R_DOWN:
            if (pp.mb || (pp.rb && pp.lb) || !pp.rb)
                return ILLEGAL;
            else return MOVE_DOWN;
            break;
        case MOVE_DOWN:
            if (pp.mb || pp.lb) return ILLEGAL;
            else if (!pp.rb) return R_UP;
            else return MOVE_DOWN;
            break;
        case R_UP:
            if (pp.mb || abs(pp.delta_x) > tolerance || abs(pp.delta_y) > tolerance)
                return ILLEGAL;
            else if (pp.lb)
                return L_DOWN;
            else if (pp.rb)
                return R_DOWN;
            else
                return NEUTRAL;
            break;
        case ILLEGAL:
            if (pp.lb) return L_DOWN;
            else return NEUTRAL;
            break;
    }
    return NEUTRAL;
}

bool (validate_line_up) (uint8_t x_len) {
    return (line_y / line_x >= 1 && line_x >= x_len);
}

bool (validate_line_down) (uint8_t x_len) {
    return (line_y / line_x <= -1 && line_x >= x_len);
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
