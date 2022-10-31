/*
 * main.c
 *
 * Created: 10/18/2022 6:15:25 PM
 *  Author: Vadim
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR (F_CPU/(16/BAUD-1))

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lora.h"

int main(void)
{
	uart_init(MYUBRR);

	if(lora_init() == 1) {
		while(1) {
			//start working
		}
	} else {
		while (1) {/*error*/}
	}

	return 0;
}