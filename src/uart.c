/*
 * uart.c
 *
 * Created: 18.10.2022 22:23:14
 *  Author: Vadim
 */

#include "uart.h"

//Инициализация UART
void uart_init(unsigned int con_speed) {
	UBRR0L = con_speed;
	
	//Разрешение на запись и чтение
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//Установка режима: 8 бит, 2 стоп бита
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

//Отправка/запись данных
void uart_transmit(unsigned char data) {
	//Ожидание пустого буфера отправки
	while(!(UCSR0A & (1<<UDRE0)));
	//Помещение данных в буфер, отправка данных
	UDR0 = data;
}

//Получение/чтение данных
unsigned char uart_receive() {
	//Ожидание данных
	while(!(UCSR0A & (1<<RXC0)));
	//Сбор данных из буфера
	return UDR0;
}