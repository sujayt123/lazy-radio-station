
#!/bin/bash

msp430-gcc -Os -std=gnu99 -g -Wall -mmcu=msp430g2553 main.c lcd.c -o main.out

