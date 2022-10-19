/*
 * uart.c
 *
 * Created: 18.10.2022 22:23:14
 *  Author: Vadim
 */

#include "uart.h"

//������������� UART
void uart_init(unsigned int con_speed) {
	UBRR0L = con_speed;
	
	//���������� �� ������ � ������
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//��������� ������: 8 ���, 2 ���� ����
	UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

//��������/������ ������
void uart_transmit(unsigned char data) {
	//�������� ������� ������ ��������
	while(!(UCSR0A & (1<<UDRE0)));
	//��������� ������ � �����, �������� ������
	UDR0 = data;
}

//���������/������ ������
unsigned char uart_receive() {
	//�������� ������
	while(!(UCSR0A & (1<<RXC0)));
	//���� ������ �� ������
	return UDR0;
}