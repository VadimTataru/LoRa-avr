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
    uart_init(UBRR);

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
            0x21,
            0x22,					//Адрес - 0000
            sped,					//0x00011010 0x1A
            0x11,					//channel (410 + (value * 1M)) = 433MHz
            option					//0x01000100 0x44
    };

    DDRE |= (1 << PE2);
    DDRE |= (1 << PE3);

    if(lora_init_with_config(cnfg) == 1) {
        lora_switch_mode(MODE_POWER_SAVING);
        while (1) {
            PORTE &= ~(1 << PE3);
            unsigned char letter = (unsigned char)uart_receive();
            if(letter == 'H') {
                PORTE |= (1 << PE3);
                _delay_ms(500);
                PORTE &= ~(1 << PE3);
            }
        };
    } else {
        while (1) {
            PORTE |= (1 << PE2);
        };
    }

    return 0;
}