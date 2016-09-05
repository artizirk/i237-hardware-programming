avr-gcc -Os -DF_CPU=16000000UL -Wall -Wextra -Wpedantic -Wformat -pedantic-errors -Werror -Wfatal-errors -mmcu=atmega2560 -c -o src/main.o src/main.c
avr-gcc -mmcu=atmega2560 src/main.o -o bin/atmega2560-user-code.elf
avr-objcopy -O ihex -R .eeprom bin/atmega2560-user-code.elf bin/atmega2560-user-code.ihx
avrdude -v -F -V -c stk500v2 -p m2560 -P /dev/ttyACM0 -b 115200 -U flash:w:bin/atmega2560-user-code.ihx
