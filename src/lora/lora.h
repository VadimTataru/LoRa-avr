/*
 * lora.h
 *
 * Created: 31.10.2022 7:31:05
 *  Author: Vadim
 */ 


#ifndef LORA_H_
#define LORA_H_

#include <avr/io.h>

#define M0                          PE5
#define M1                          PE6
#define DDR_E		                DDRE
#define PORT_E	                    PORTE

//Registers
#define REG_FIFO                    0x00
#define REG_OP_MODE                 0x01
#define REG_FIFO_TX_BASE_ADDR       0x0e
#define REG_FIFO_RX_BASE_ADDR       0x0f
#define REG_LNA                     0x0c
#define REG_PA_CONFIG               0x09
#define REG_MODEM_CONFIG_1          0x1d
#define REG_MODEM_CONFIG_2          0x1e
#define REG_MODEM_CONFIG_3          0x26
#define REG_VERSION                 0x42
#define REG_PAYLOAD_LENGTH          0x22
#define REG_RX_NB_BYTES             0x13
/*#defineREG_TEST0x1a*/

//modes
#define MODE_LONG_RANGE_MODE        0x80
#define MODE_SLEEP                  0x00
#define MODE_STDBY                  0x01

//PA config
#define PA_BOOST                    0x80

//package
#define MAX_PKT_LENGTH              255

enum HEAD_TYPE {
    SAVE_CNFG = 0xC0,
    UNSAVE_CNFG = 0xC2
}

struct Speed
{
    uint8_t air_data_rt: 3;
    uint8_t uart_baud_rt: 3;
    uint8_t uart_parity: 2;
};

struct Option
{
    uint8_t transmission_power: 2;  //Transmission power      
    uint8_t fec: 1;                 //FEC
    uint8_t wrlss_wkp_time: 3;      //wireless wake up time
    uint8_t io_drv_mode: 1;         //IO drive mode
    uint8_t fxed_transmittion: 1;   //fixed transmission
};


typedef struct
{
    enum HEAD_TYPE HEAD;
    uint8_t ADDH;
    uint8_t ADDL;
    //struct Speed SPED;
    uint8_t SPED;
    uint8_t CHAN;
    uint8_t OPTION;
    //struct Option OPTRION;    
} Config;

uint8_t lora_init();

uint8_t lora_init_with_config(Config cnfg);

uint8_t lora_check_version();

void lora_set_frequency(uint32_t freq);

void lora_stanby();

void lora_sleep_mode();

void set_tx_power(uint8_t level);

void set_address(uint8_t add_tx, uint8_t add_rx);

uint8_t writeMessage(const char *buffer, uint8_t size)

int8_t readMessage();


#endif /* LORA_H_ */