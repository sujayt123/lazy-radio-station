//***************************************************************************************
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
 
    lcd_init(0);
    lcd_clear_all();
    lcd_display_string(0, descriptions[0]);

    P1REN |= BIT1;
    P1OUT |= BIT1; /* Enable internal pull up resistors */

  int i = 0;
  int pirState = 0;
  shiftOut(~(1 << i));    
  
  while(1)
  {
    if (!(P1IN & BIT1)) // Prioritize button press over motion
    {
      shiftOut(~(1 << i));  
      lcd_clear_all();
      lcd_display_string(0, descriptions[i]);
     __delay_cycles(500000);
     i = (i + 1) % 8;               
    }
    else if (P1IN & BIT2)
    {
    	if (pirState == 0)
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
  if (val){
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
// LED's when you have the enable pin tied to the MCU. Entirely optional.
void enable( void )
{
  P1OUT &= ~ENABLE;
}
 
void disable( void )
{
  P1OUT |= ENABLE;
}