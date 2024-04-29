#include "rtc.h"
#include "rtc_macros.h"

bool game_in_progress = true;
int rtc_hook_id = 0;

int (rtc_write_to_reg) (uint32_t reg, uint32_t data) {
    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    if (sys_outb(RTC_DATA_REG, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_read_from_reg) (uint32_t reg, uint8_t* data) {
    if (sys_outb(RTC_ADDR_REG, reg) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    if (util_sys_inb(RTC_DATA_REG, data) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_set_alarm) () {
    uint8_t reg_b, reg_a, hours, minutes, seconds;

    if (rtc_read_from_reg(RTC_REG_B, &reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    reg_b |= RTC_AIE | RTC_SET | RTC_DM;

    do {
        if (rtc_read_from_reg(RTC_REG_A, &reg_a) != 0) {
            printf("ERROR: %s\n", __func__);
            return 1;
        }
    } while ((reg_a & RTC_UIP) != 0);

    if (rtc_write_to_reg(RTC_REG_B, reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    // rtc_get_time
    if (rtc_read_from_reg(RTC_HOURS, &hours) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_read_from_reg(RTC_MINS, &minutes) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_read_from_reg(RTC_SECS, &seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    // rtc_set_alarm
    if (rtc_write_to_reg(RTC_HOURS_ALARM, hours) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_write_to_reg(RTC_MINS_ALARM, minutes + 1) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_write_to_reg(RTC_SECS_ALARM, seconds) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    if (rtc_read_from_reg(RTC_REG_B, &reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    reg_b &= RTC_RESET;

    if (rtc_write_to_reg(RTC_REG_B, reg_b) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }

    return 0;
}

void (rtc_ih) () {
    uint8_t reg_c;
    rtc_read_from_reg(RTC_REG_C, &reg_c);

    if (reg_c & RTC_AF) game_in_progress = false;
}

int (rtc_subscribe_int) (uint8_t* bit_no) {
    rtc_hook_id = *bit_no;
    if (sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}

int (rtc_unsubscribe_int) () {
    if (sys_irqrmpolicy(&rtc_hook_id) != 0) {
        printf("ERROR: %s\n", __func__);
        return 1;
    }
    return 0;
}
