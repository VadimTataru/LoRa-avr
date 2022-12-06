/*
 * uart.c
 *
 * Created: 18.10.2022 22:23:14
 *  Author: Vadim
 */

#include "uart.h"

/*----------------------------------------------------------------------
 Концигурирование UART
 TX на порту PE1, RX на порту PE0
 Длина пакета 8 бит + 2 стоп бит.
----------------------------------------------------------------------*/
void uart_init(uint8_t con_speed) {
	DDRE &= ~(1 << PE0);        // RX
    DDRE |=  (1 << PE1);        // TX

	UBRR0H = (uint8_t)(con_speed >> 8);
	UBRR0L = con_speed;
	
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//Установка режима: 8 бит, 2 стоп бита
	UCSR0C = 	(1 << USBS0)   //0 - one stop bit, 1 - two stop bits
           	  | (0 << UCSZ02)  | (1 << UCSZ01) | (1 << UCSZ00);  //8 bit
}

/*----------------------------------------------------------------------
 Запись в регистр UDR0 -> отправка бита
----------------------------------------------------------------------*/
void uart_transmit(uint8_t data) {
	//Ожидание пустого буфера отправки
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

/*----------------------------------------------------------------------
 Чтение регистра UDR0 -> получение бита
----------------------------------------------------------------------*/
uint8_t uart_receive() {
	//Ожидание данных
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

/*----------------------------------------------------------------------
 Отправка сообщения
----------------------------------------------------------------------*/
void uart_transmit_serial(uint8_t *data) {
	for(int i = 0; i < sizeof(data); i++) {
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = data[i];
	}
}

uint8_t uart_write_register(uint8_t reg, uint8_t value) {
	uart_transmit(reg | 0x80);
	uart_transmit(value);
}

uint8_t uart_read_register(uint8_t reg) {
	uart_transmit(reg & 0x7f);
	uart_transmit(0x00);
	return uart_receive();
}