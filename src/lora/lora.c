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
        uart_write_register(REG_FIFO_TX_BASE_ADDR, 0);
        uart_write_register(REG_FIFO_RX_BASE_ADDR, 0);
        uart_write_register(REG_LNA, uart_read_register(REG_LNA) | 0x03);
        uart_write_register(REG_MODEM_CONFIG_3, 0x04);                          // set auto AGC
        set_tx_power(17);
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
    uart_write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void lora_sleep() {
    uart_write_register(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void set_tx_power(uint8_t level) {
    if(level < 2) 
        level = 2;
    else if(level > 17)
        level = 17;
    
    uart_write_register(REG_PA_CONFIG, PA_BOOST | (level - 2));
}