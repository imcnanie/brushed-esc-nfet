# brushed-esc-nfet

This is firmware to convert all-nfet brushless ESCs to brushed. Can work for normal SimonK-capable ESCs with slight modifications.

## Compiling
```
avr-gcc -g -mmcu=atmega8 -Wall -Os -c main.c
avr-gcc -g -mmcu=atmega8 -o main.out -Wl,-Map,main.map main.o
avr-objcopy -R .eeprom -O ihex main.out main.hex
avr-size main.out

sudo avrdude -P /dev/ttyACM0 -b 19200 -c arduino -p m8 -v -e -U flash:w:"main.hex":i
```
## Pinout ![pinout](IMG_20161214_174532.jpg)
