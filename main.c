/*
 * main.c
 *
 * Created: 10/18/2022 6:15:25 PM
 *  Author: Vadim
 */

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lora.h"

int main(void)
{
    uint8_t btn_ticks_counter = 0; // Счетчик тиков удержания кнопки
	uart_init(UBRR);

    Speed sped = {
        {
            0b010,
            0b011,
            0b00,
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
        SAVE_CNFG,                //0xC0 - сохраняем настройки при отключении питания
        0x13,
        0xAC,                    //Адрес - 0000
        sped, //0x00011010 0x1A
        0x17,                    //channel (410 + (value * 1M)) = 433MHz
        option    //0x01000100 0x44
    };

    DDRE |= (1 << PE2);
    DDRE |= (1 << PE3);

    DDRE &= ~(1 << PE4); //Настройка вывода PE4 на вход для считывания показаний кнопки
    PORTE |= (1 << PE4);
    PORTE &= ~(1 << PE3);

    if (lora_init_with_config(cnfg) == 1) {
        lora_switch_mode(MODE_WAKE_UP);
        while (1) {

            if((PINE & (1 << PE4)) == 0) {
                _delay_ms(10);
                if (btn_ticks_counter == 0)
                {
                    uart_transmit('H');
                }
                // Увеличиваем счетчик тиков, пока он не станет равен 10
                if (btn_ticks_counter < 10)
                {
                    btn_ticks_counter++; // Увеличиваем на 1 счетчик тиков
                }
            } else {
                btn_ticks_counter = 0;
            }
        };
    } else {
        while (1) {
            PORTE |= (1 << PE2);
        };
    }

    return 0;
}