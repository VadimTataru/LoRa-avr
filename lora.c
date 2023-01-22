/*
 * lora.c
 *
 * Created: 31.10.2022 7:31:14
 *  Author: Vadim
 */

#include "lora.h"
#include "uart.h"

uint8_t packetIndex = 0;
unsigned char divider = ';';
/*----------------------------------------------------------------------
 C3 32 xx yy; 
 32 - модель модуля (E32), xx - номер версии радиомодуля, yy - относится к другим функциям модуля
----------------------------------------------------------------------*/
uint8_t lora_version_data[4];

uint8_t lora_init() {

    //Стоит вынести в define
    Config cnfg = {
		SAVE_CNFG, 		//0xC0 - сохраняем настройки при отключении питания
		0x00,
		0x00,			//Адрес - 0000
        {
            {
                0b000,
                0b110,
                0b10
            }
        },			//0x00011010
		0x17,			//channel (410 + (value * 1M)) = 433MHz
        {
            {
                0b01,
                0b0,
                0b001,
                0b0,
                0b0
            }
        }			//0x01000100
	};

    DDR_MODE |= (1 << M0);
    DDR_MODE |= (1 << M1);

    PORT_MODE &= ~(1 << M0);
    PORT_MODE &= ~(1 << M1);

    if(!lora_check_version()) 
        return 0;
    lora_switch_mode(MODE_SLEEP);

    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.HEAD);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.ADDH);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.ADDH);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.SPED.sped);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.CHAN);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.OPTION.options);

    lora_switch_mode(MODE_NORMAL);
    return 1;
}

/*----------------------------------------------------------------------
 Инициализация радиомодуля с пользовательскими настройками
----------------------------------------------------------------------*/
uint8_t lora_init_with_config(Config cnfg) {

    DDR_MODE |= (1 << M0);
    DDR_MODE |= (1 << M1);

    PORT_MODE &= ~(1 << M0);
    PORT_MODE &= ~(1 << M1);

    if(!lora_check_version()) 
        return 0;

    lora_switch_mode(MODE_SLEEP);

    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.HEAD);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.ADDH);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.ADDH);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.SPED.sped);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.CHAN);
    _delay_ms(2);
    uart_transmit((uint8_t)cnfg.OPTION.options);

    lora_switch_mode(MODE_NORMAL);
    return 1;
}

/*----------------------------------------------------------------------
 Версия радиомодуля
----------------------------------------------------------------------*/
uint8_t lora_check_version() {
    lora_switch_mode(MODE_SLEEP);
    uint8_t params[4];

    _delay_ms(2);
    uart_transmit((uint8_t)0xC3);
    _delay_ms(2);
    uart_transmit((uint8_t)0xC3);
    _delay_ms(2);
    uart_transmit((uint8_t)0xC3);

    lora_switch_mode(MODE_NORMAL);
    
    return 1;
}

/*----------------------------------------------------------------------
 Просмотр сохранённых параметров
----------------------------------------------------------------------*/
uint8_t lora_get_saved_params() {
    lora_switch_mode(MODE_SLEEP);
    uint8_t params[6];
    _delay_ms(50);
    
    _delay_ms(2);
    uart_transmit(0xC1);
    _delay_ms(2);
    uart_transmit(0xC1);
    _delay_ms(2);
    uart_transmit(0xC1);
    
    _delay_ms(40);
    lora_switch_mode(MODE_NORMAL);
    return 1;
}

/*----------------------------------------------------------------------
 Просмотр сохранённых параметров
----------------------------------------------------------------------*/
uint8_t lora_reset_config() {
    lora_switch_mode(MODE_SLEEP);

    _delay_ms(2);
    uart_transmit(0xC4);
    _delay_ms(2);
    uart_transmit(0xC4);
    _delay_ms(2);
    uart_transmit(0xC4);

    lora_switch_mode(MODE_NORMAL);
    
    return 1;
}

uint8_t set_transmition_mode(TRNSM_MODE mode) {
    uint8_t * params[6] = lora_get_saved_params();

    Option options = {
        {

        },
        params[5]
    };

    switch (mode)
    {
    case FIXED_TRANSM:
        options.bits.fxed_transmittion = 0b1;
        break;
    
    case TRANSPARENT_TRANSM:
        options.bits.fxed_transmittion = 0b0;
        break;
    }
    params[5] = options.options;
    lora_switch_mode(MODE_SLEEP);
    uart_transmit_serial(params);
    lora_switch_mode(MODE_NORMAL);

    //Возвращать адекватное значение
    return 1;
}

/*----------------------------------------------------------------------
 Переключение между режимами работы радиомодуля
----------------------------------------------------------------------*/
void lora_switch_mode(LORA_MODE mode) {
    switch (mode)
    {
    case MODE_NORMAL:
        PORT_MODE &= ~(1 << M0);
        PORT_MODE &= ~(1 << M1);
        break;
    case MODE_WAKE_UP:
        PORT_MODE |= (1 << M0);
        PORT_MODE &= ~(1 << M1);
        break;
    case MODE_POWER_SAVING:
        PORT_MODE &= ~(1 << M0);
        PORT_MODE |= (1 << M1);
        break;
    case MODE_SLEEP:
        PORT_MODE |= (1 << M0);
        PORT_MODE |= (1 << M1);
        break;    
    default:
        break;
    }
}

/*----------------------------------------------------------------------
 Отправка сообщения по радиомодулю
----------------------------------------------------------------------*/
uint8_t sendMessage(const char *buffer, uint8_t size) {
    //TODO: Добавить обработку преамбул и Header
    if(size == 0) return 0;
    lora_switch_mode(MODE_NORMAL);

    uart_transmit_serial(buffer);

    //FixMe: возвращать адекватное значение; отказаться от size?
    return size;

    // uint16_t currentLength = uart_read_register(REG_PAYLOAD_LENGTH);
    // if ((currentLength + size) > MAX_PKT_LENGTH)                // check size
    //     size = MAX_PKT_LENGTH - currentLength;
    // for (uint8_t i=0; i<size; i++) {                         // write data
    //     uart_write_register(REG_FIFO, buffer[i]);
    // }
    // uart_write_register(REG_PAYLOAD_LENGTH, currentLength + size);    // update length
    // return size;
}

int8_t available() {
    // сужающее преобразование(uint8_t -> int8_t)
    return (int8_t)(uart_read_register(REG_RX_NB_BYTES) - packetIndex);
}

int8_t readMessage() {
    if (!available())
        return -1;
    packetIndex++;
    // сужающее преобразование(uint8_t -> int8_t)
    return (int8_t)uart_read_register(REG_FIFO);
}

unsigned char * getMessage() {
    uint8_t counter = 0;
    uint8_t i = 0;
    unsigned char * result;
    unsigned char * dividers;
    while (counter < 3)
    {
        unsigned char data_byte = uart_receive();
        if(data_byte == divider) {
            dividers[counter] = data_byte;
            counter++;            
        } else {
            if(sizeof(dividers) > 0) {
                result = dividers;
                i = sizeof(dividers);
                result[i] = data_byte;
                i++;
                //удалить dividers
            }
                
        }
            
    }
    
}