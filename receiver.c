/*
 * receiver.c
 *
 * Created: 01/13/2023 7:36:17 PM
 *  Author: Vadim
 */


#include "lora.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR F_CPU/16/BAUD-1

int main(void) {
    uart_init(UBRR)

    Speed sped = {
            {
                    0b010,
                    0b011,
                    0b00
            }
    };

    Option option = {
            {
                    0b00,
                    0b1,
                    0b000,
                    0b1,
                    0b0
            }
    };

    //Default config
    Config cnfg = {
            SAVE_CNFG, 				//0xC0 - сохраняем настройки при отключении питания
            0x00,
            0x00,					//Адрес - 0000
            sped,					//0x00011010 0x1A
            0x17,					//channel (410 + (value * 1M)) = 433MHz
            option					//0x01000100 0x44
    };

    if(lora_init_with_config(cnfg) == 1) {
        while (1) {
            /*do work*/
        };
    } else {
        while (1) {
            /*error*/
        };
    }

    return 0;
}