/*
 * uart.h
 *
 * Created: 18.10.2022 18:19:52
 *  Author: Vadim
 */ 

#ifndef UART_H_
#define UART_H_

#define  TX0		PE1
#define  DDR_TX		DDRE
#define  PORT_TX	PORTE 

#define	 RX0		PE0
#define  DDR_RX		DDRE
#define  PORT_RX	PORTE

/*
 * Инициализация uart
 */
void uart_init(unsigned int con_speed);

/*
 * Отправка данных
 */
void uart_transmit(unsigned char data);

/*
 * Получение данных
 */
unsigned char uart_receive();

#endif /* UART_H_ */