# Lazy Man's Radio

You’ve just come home from your 12 hour shift at Kemere Labs. It's time to lay back and destress with some of your favorite tunes. To your dismay, you find that changing songs requires an inordinate amount of effort. Who has time to reach over for the darned skip button every time she wants to move on to the next song?

That’s where we come in!

Presenting, the lazy man’s radio. An inexpensive and modern alternative to today’s music players. Simply wave your hand in front of the sensor to trigger a “skip” to the next song! Our music player features all the standard capabilities of its contemporaries for a fraction of the price.



### Features

* An LCD screen to display information about the current track
* A PIR sensor to detect motion and subsequently trigger the next track
* A pushbutton that also allows you to cycle through tracks
* A pretty LED display to further indicate which track is on
* A shift register to configure the means of flipping tracks
  * Modify and reprogram the system to skip one, two or more tracks per motion
  * Reorder the order in which to visit songs on the soundboard
* A (customizable) soundboard pre-loaded with eight of today's most popular hits


### Partslist

 * [16x2 HD44780 LCD Screen](https://www.adafruit.com/product/181) to display track info
 * [74HC595 Shift Register](http://www.ti.com/lit/ds/symlink/sn74hc595.pdf) to serve as a 1-cold decoder
 * [Pushbutton](https://www.sparkfun.com/products/97) for track cycling
 * [Audio FX Soundboard](https://www.adafruit.com/product/2220) to store the uncompressed audio files
 * [TI-MSP430G2553 Launchpad](http://www.ti.com/tool/MSP-EXP430G2) to program, control and power the embedded system
 * [PIR Sensor](https://www.adafruit.com/product/189) for motion detection
 * [5V/1A USB power supply](https://www.amazon.com/Jackery-Premium-3350mAh-Portable-Charger/dp/B00L9F95RO/ref=zg_bs_7073960011_15?_encoding=UTF8&psc=1&refRID=KQ9980D06M4S5FD3NESW) to power the speaker and soundboard
 * [Speaker](https://www.sparkfun.com/products/14023) to emit the audio output of the system

### Schematic

### Architecture
##### Hardware

##### Software

### Discussion

### Code
[Link to Github repo](github.com/sujayt123)
###### Implementation of main.c
```
//***************************************************************************************
// Sujay Tadwalkar, et al.
//
// Some code adapted from http://processors.wiki.ti.com/index.php/MSP430_Launchpad_Shift_Register
//
// LCD device driver adapted from https://github.com/LINKIWI/msp430-hd44780u
//***************************************************************************************
#include <msp430.h>

#include "lcd.h"

//Define our pins
#define DATA BIT0 // DS -> 1.0
#define CLOCK BIT4 // SH_CP -> 1.4
#define LATCH BIT5 // ST_CP -> 1.5
#define ENABLE BIT6 // OE -> 1.6
// The OE pin can be tied directly to ground, but controlling
// it from the MCU lets you turn off the entire array without
// zeroing the register

// Declare functions
void delay ( unsigned int );
void pulseClock ( void );
void shiftOut ( unsigned char );
void enable ( void );
void disable ( void );
void init ( void );
void pinWrite ( unsigned int, unsigned char );

char *descriptions[] = {
    "Tune 1",
    "Tune 2",
    "Tune 3",
    "Tune 4",
    "Tune 5",
    "Tune 6",
    "Tune 7",
    "Tune 8"
};

int main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    if (CALBC1_1MHZ == 0xFF || CALDCO_1MHZ == 0xff)
        while (1); // Erased calibration data? Trap!

    BCSCTL1 = CALBC1_1MHZ; // Set the DCO to 1 MHz; MCLK sources this
    DCOCTL = CALDCO_1MHZ; // And load calibration data

    P1DIR |= (DATA + CLOCK + LATCH + ENABLE);  // Setup pins as outputs
    enable(); // Enable output (pull OE low)

    lcd_init(0);		// Initialize the LCD screen
    lcd_clear_all();
    lcd_display_string(0, descriptions[0]);

    P1REN |= BIT1;
    P1OUT |= BIT1; /* Enable internal pull up resistors */

    int i = 0;
    int pirState = 0;
    shiftOut(~(1 << i));    /* Play the first track */

    while(1)
    {
        if (!(P1IN & BIT1)) // Prioritize button press over motion
        {
            shiftOut(~(1 << i));  // Trigger the current song
            lcd_clear_all();
            lcd_display_string(0, descriptions[i]); // Display the current song
            __delay_cycles(500000);	// Allow the switch to debounce
            i = (i + 1) % 8;           // Identify the next song to play
        }
        else if (P1IN & BIT2)	// The IR sensor output is high
        {
            if (pirState == 0)	// Only trigger on the "rising edge" of the output
            {
                shiftOut(~(1 << i));
                lcd_clear_all();
                lcd_display_string(0, descriptions[i]);
                __delay_cycles(500000);
                i = (i + 1) % 8;
                pirState = 1;
            }
        }
        else
        {
            pirState = 0;
        }
    }
}

// Delays by the specified Milliseconds
// thanks to:
// http://www.threadabort.com/archive/2010/09/05/msp430-delay-function-like-the-arduino.aspx
void delay(unsigned int ms)
{
    while (ms--)
    {
        __delay_cycles(1000); // set for 16Mhz change it to 1000 for 1 Mhz
    }
}

// Writes a value to the specified bitmask/pin. Use built in defines
// when calling this, as the shiftOut() function does.
// All nonzero values are treated as "high" and zero is "low"
void pinWrite( unsigned int bit, unsigned char val )
{
    if (val) {
        P1OUT |= bit;
    } else {
        P1OUT &= ~bit;
    }
}

// Pulse the clock pin
void pulseClock( void )
{
    P1OUT |= CLOCK;
    P1OUT ^= CLOCK;

}

// Take the given 8-bit value and shift it out, LSB to MSB
void shiftOut(unsigned char val)
{
    //Set latch to low (should be already)
    P1OUT &= ~LATCH;

    char i;

    // Iterate over each bit, set data pin, and pulse the clock to send it
    // to the shift register
    for (i = 0; i < 8; i++)  {
        pinWrite(DATA, (val & (1 << i)));
        pulseClock();
    }

    // Pulse the latch pin to write the values into the storage register
    P1OUT |= LATCH;
    P1OUT &= ~LATCH;
}

// These functions are just a shortcut to turn on and off the array of
// LEDs when you have the enable pin tied to the MCU. Entirely optional.
void enable( void )
{
    P1OUT &= ~ENABLE;
}

void disable( void )
{
    P1OUT |= ENABLE;
}
```
