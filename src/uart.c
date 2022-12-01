/*
 * uart.c
 *
 * Created: 18.10.2022 22:23:14
 *  Author: Vadim
 */

#include "uart.h"

//Инициализация UART
void uart_init(uint8_t con_speed) {
	DDRE0 &= ~(1 << PE0);        // RX
    DDRE0 |=  (1 << PE1);        // TX

	UBRR0L = con_speed;
	
	//Разрешение на запись и чтение
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//Установка режима: 8 бит, 2 стоп бита
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

//Отправка/запись данных
void uart_transmit(uint8_t data) {
	//Ожидание пустого буфера отправки
	while(!(UCSR0A & (1<<UDRE0)));
	//Помещение данных в буфер, отправка данных
	UDR0 = data;
}

//Получение/чтение данных
uint8_t uart_receive() {
	//Ожидание данных
	while(!(UCSR0A & (1<<RXC0)));
	//Сбор и возврат данных из буфера
	return UDR0;
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