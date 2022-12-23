APP_NAME := lora_avr

AVRGCC := avr-gcc
AVROBJCOPY := avr-objcopy
AVRDUDE := avrdude

MMCU := atmega128rfa1

FLAGS := -Os -Wall -std=gnu11 -mmcu=$(MMCU)

MMCU_PROG := m128rfa1
PROGRAMMER := usbasp

source_dir := .
source_files :=		$(wildcard $(addsuffix /*.c, $(source_dir) ) )

.PHONY: all clean

#Компиляция программы прошивки
all: clean $(APP_NAME).o
	$(AVROBJCOPY) -j .text -j .data -O ihex $(APP_NAME).o $(APP_NAME).hex

$(APP_NAME).o: $(source_files)
	$(AVRGCC) $(FLAGS) $^ -o $(APP_NAME).o

#Загрузка программы на МК
flush:
	$(AVRDUDE) -c $(PROGRAMMER) -P usb -p $(MMCU_PROG) -U flash:w:$(APP_NAME).hex

#Программирование фьюзов
fuse:
	$(AVRDUDE) -c $(PROGRAMMER) -P usb -p $(MMCU_PROG) -F -U lfuse:w:0xbf:m
	$(AVRDUDE) -c $(PROGRAMMER) -P usb -p $(MMCU_PROG) -F -U lfuse:w:0x55:m
ifeq ($(OS),Windows_NT)
clean:
	cmd //C del //Q //F *.o *.hex
else
clean:
	rm -f ./*.o ./*hex
endif