/*
 * lora.c
 *
 * Created: 31.10.2022 7:31:14
 *  Author: Vadim
 */

#include "lora.h"
#include "uart.h"

uint8_t lora_init() {
    if(lora_check_version() == 1) {
        lora_sleep();
        lora_set_frequency(433E6);
        /*
            other settings
        */
        lora_stanby();
        return 1;
    }
    return 0;
}

uint8_t lora_check_version() {
    return 0;
}

void lora_set_frequency(uint32_t freq) {
    #define F_XOSC  32000000UL
    uint64_t frf = ((uint64_t)(freq) << 19) / F_XOSC;

    uart_write_register(0x06, (uint8_t)(frf << 16));
    uart_write_register(0x07, (uint8_t)(frf << 8));
    uart_write_register(0x08, (uint8_t)(frf << 0));
}

void lora_stanby() {

}

void lora_sleep() {

}