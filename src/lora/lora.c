/*
 * lora.c
 *
 * Created: 31.10.2022 7:31:14
 *  Author: Vadim
 */

#include "lora.h"
#include "uart.h"

uint8_t packetIndex = 0;

uint8_t lora_init() {

    if(!lora_check_version()) 
        return 0;
    lora_sleep();

    lora_set_frequency(433E6);
    set_address(0, 0);
    uart_write_register(
        REG_LNA, 
        uart_read_register(REG_LNA) | 0x03
    );
    uart_write_register(REG_MODEM_CONFIG_3, 0x04);
    set_tx_power(17);

    lora_stanby();
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

    uart_transmit(cnfg.HEAD);
    uart_transmit(cnfg.ADDH);
    uart_transmit(cnfg.ADDL);
    uart_transmit(cnfg.SPED.sped);
    uart_transmit(cnfg.CHAN);
    uart_transmit(cnfg.OPTIONS.options);

    lora_switch_mode(MODE_NORMAL);
    return 1;
}

uint8_t lora_check_version() {
    lora_switch_mode(MODE_SLEEP);

    //Переделать отправку данных
    uart_transmit(0xC3);
    uart_transmit(0xC3);
    uart_transmit(0xC3);

    lora_switch_mode(MODE_NORMAL);
    return uart_receive();

    //uint8_t version = uart_read_register(REG_VERSION);
    //return version == 0x12 ? 1 : 0;
}

/*----------------------------------------------------------------------
 Просмотр сохранённых параметров
----------------------------------------------------------------------*/
uint8_t lora_get_saved_params() {
    lora_switch_mode(MODE_SLEEP);

    //Переделать отправку данных
    uart_transmit(0xC1);
    uart_transmit(0xC1);
    uart_transmit(0xC1);

    lora_switch_mode(MODE_NORMAL);
    return uart_receive();
}

/*----------------------------------------------------------------------
 Просмотр сохранённых параметров
----------------------------------------------------------------------*/
uint8_t lora_reset_config() {
    lora_switch_mode(MODE_SLEEP);

    //Переделать отправку данных
    uart_transmit(0xC4);
    uart_transmit(0xC4);
    uart_transmit(0xC4);

    lora_switch_mode(MODE_NORMAL);
    return uart_receive();
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

void lora_set_frequency(uint32_t freq) {
    #define F_XOSC  32000000UL
    uint64_t frf = ((uint64_t)(freq) << 19) / F_XOSC;

    uart_write_register(0x06, (uint8_t)(frf << 16));
    uart_write_register(0x07, (uint8_t)(frf << 8));
    uart_write_register(0x08, (uint8_t)(frf << 0));
}

void set_tx_power(uint8_t level) {
    if(level < 2) 
        level = 2;
    else if(level > 17)
        level = 17;
    
    uart_write_register(REG_PA_CONFIG, PA_BOOST | (level - 2));
}

void set_address(uint8_t add_tx, uint8_t add_rx) {
    uart_write_register(REG_FIFO_TX_BASE_ADDR, add_tx);
    uart_write_register(REG_FIFO_RX_BASE_ADDR, add_rx);
}

uint8_t writeMessage(const char *buffer, uint8_t size) {
    uint16_t currentLength = uart_read_register(REG_PAYLOAD_LENGTH);
    if ((currentLength + size) > MAX_PKT_LENGTH)                // check size
        size = MAX_PKT_LENGTH - currentLength;
    for (uint8_t i=0; i<size; i++) {                         // write data
        uart_write_register(REG_FIFO, buffer[i]);
    }
    uart_write_register(REG_PAYLOAD_LENGTH, currentLength + size);    // update length
    return size;
}

int8_t available() {
    return (uart_read_register(REG_RX_NB_BYTES) - packetIndex);
}

int8_t readMessage() {
    if (!available())
        return -1;
    packetIndex++;
    return uart_read_register(REG_FIFO);
}
