/*
 * main.c
 *
 * Created: 10/18/2022 6:15:25 PM
 *  Author: Vadim
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lora.h"

int main(void)
{
	uart_init(MYUBRR);

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
			uart_transmit('H');
			_delay_ms(1000);
			uart_transmit('E');
			_delay_ms(1000);
			uart_transmit('L');
			_delay_ms(1000);
			uart_transmit('L');
			_delay_ms(1000);
			uart_transmit('O');
			_delay_ms(1000);
		};
	} else {
		while (1) {
			/*error*/
		};
	}

	return 0;
}