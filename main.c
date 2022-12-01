/*
 * main.c
 *
 * Created: 10/18/2022 6:15:25 PM
 *  Author: Vadim
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define num 0x44

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lora.h"

int main(void)
{
	uart_init(MYUBRR);

	//Default config
	Config cnfg = {
		SAVE_CNFG, 		//0xC0 - сохраняем настройки при отключении питания
		0x00,
		0x00,			//Адрес - 0000
		0x1A,			//0x00011010
		0x17,			//channel (410 + (value * 1M)) = 433MHz
		0x44			//0x01000100
	};

	if(lora_init_with_config(cnfg) == 1) {
		while(1) {
			uart_transmit('H');
			_delay(1000);
			uart_transmit('E');
			_delay(1000);
			uart_transmit('L');
			_delay(1000);
			uart_transmit('L');
			_delay(1000);
			uart_transmit('O');
			_delay(1000);
		}
	} else {
		while (1) {/*error*/}
	}

	return 0;
}