/*
 * lora.h
 *
 * Created: 31.10.2022 7:31:05
 *  Author: Vadim
 */ 


#ifndef LORA_H_
#define LORA_H_

#include <avr/io.h>

//������������� LoRa
uint8_t lora_init();

//��� ������ �������
uint8_t lora_check_version();

void lora_set_frequency(uint32_t freq);

void lora_stanby();

void lora_sleep();


#endif /* LORA_H_ */