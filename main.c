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

int main(void)
{
	uart_init(MYUBRR);

	for (;;)
	{
		_delay_ms(1000);
		uart_transmit('a')
	}


	return 0;
}