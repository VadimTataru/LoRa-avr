/*
 * lora.h
 *
 * Created: 31.10.2022 7:31:05
 *  Author: Vadim
 */ 


#ifndef LORA_H_
#define LORA_H_

#include <avr/io.h>

//Registers
#define REG_OP_MODE                 0x01
#define REG_FIFO_TX_BASE_ADDR       0x0e
#define REG_FIFO_RX_BASE_ADDR       0x0f
#define REG_LNA                     0x0c
#define REG_PA_CONFIG               0x09
#define REG_MODEM_CONFIG_1          0x1d
#define REG_MODEM_CONFIG_2          0x1e
#define REG_MODEM_CONFIG_3          0x26

//modes
#define MODE_LONG_RANGE_MODE        0x80
#define MODE_SLEEP                  0x00
#define MODE_STDBY                  0x01

//PA config
#define PA_BOOST                    0x80

//������������� LoRa
uint8_t lora_init();

//��� ������ �������
uint8_t lora_check_version();

void lora_set_frequency(uint32_t freq);

void lora_stanby();

void lora_sleep();

void set_tx_power(uint8_t level);


#endif /* LORA_H_ */