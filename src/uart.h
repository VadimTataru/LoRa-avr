/*
 * uart.h
 *
 * Created: 18.10.2022 18:19:52
 *  Author: Vadim
 */ 

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

#define  TX0		PE1
#define  DDR_TX		DDRE
#define  PORT_TX	PORTE 

#define	 RX0		PE0
#define  DDR_RX		DDRE
#define  PORT_RX	PORTE

/*
 * ������������� uart
 */
void uart_init(uint8_t con_speed);

/*
 * �������� ������
 */
void uart_transmit(uint8_t data);

/*
 * ��������� ������
 */
uint8_t uart_receive();

uint8_t uart_write_register(uint8_t reg, uint8_t value);

uint8_t uart_read_register(uint8_t reg);


#endif /* UART_H_ */