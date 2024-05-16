#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdint.h>
#include <assert.h>

uint32_t sysinb_counter;

int (util_sys_inb)(int port, uint8_t *value) {

    uint32_t new_val = *value;

    assert(sys_inb(port, &new_val) == 0);

    *value = new_val & 0x000000FF;
    return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    *lsb = (uint8_t) (val & 0x00FF);
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    *msb = (uint8_t) ((val & 0xFF00) >> 8);
    return 0;
}
