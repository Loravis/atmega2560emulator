NAME = program
MMCU = atmega2560
BAUDRATE = 115200

$(NAME).hex: $(NAME).s
	avr-gcc -mmcu=$(MMCU) -c $< -o $(NAME).o
	avr-gcc -mmcu=$(MMCU) $(NAME).o -o $(NAME).elf
	avr-objcopy -O ihex $(NAME).elf $@
upload: $(NAME).hex
	avrdude -v -p atmega2560 -c wiring -P /dev/ttyACM0 -b $(BAUDRATE) -D -U flash:w:$<:i
dump: $(NAME).hex
	avr-objdump -s -m $(MMCU) $<
