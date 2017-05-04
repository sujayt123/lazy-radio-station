/**
 * Functions to control the LCD.
 */

#include <msp430g2553.h>
#include <stdint.h>
#include <string.h>

#include "lcd.h"


/* Define encodings for special instructions that can be sent to the LCD. */
#define LCD_INSTRUCTION_CLEAR_DISPLAY 0x01
#define LCD_INSTRUCTION_DISPLAY_ON 0x0C
#define LCD_INSTRUCTION_RETURN_HOME 0x02

/* Define the number of cycles to wait before transmitting more data to the LCD. */
#define TRANSMISSION_DELAY 1000

/* Define utility macros. */
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

/* Global variables necessary to keep track of the amount of delay artifically imposed on the LCD. */
static int wait_cycles = 0;
static int cycle_threshold = 0;


/* Private function prototypes */
static void lcd_tx_4_bit(char data);
static void lcd_tx_8_bit(char data);
static void lcd_set_line(int line);
static void lcd_display_char(char c);
static void lcd_instruction(char instruction);


/**
 * Initialize the LCD. This function must be called before any other LCD control functions. This behavior of this
 * function is not idempotent; calling it multiple times will have the effect of resetting anything currently
 * displayed on the screen.
 */
void lcd_init(int cycle_delay_threshold) {
    // This function is wrapped with artificial delays for purposes of voltage stabilization.
    __delay_cycles(50000);

    // Keeps track of the threshold number of cycles before allowing the LCD to be updated.
    cycle_threshold = cycle_delay_threshold;

    // All relevant LCD control pins should be designated as output and pulled low.
    LCD_PORT_DIR = LCD_RS | LCD_EN | LCD_DATA;
    LCD_PORT_OUT &= ~(LCD_RS | LCD_EN | LCD_DATA);

    // Initialize the LCD configuration.
    lcd_tx_8_bit(
        0x20 |  // Set the LCD function to a 5x8 dot character font
        0x08  // This LCD has two lines of display
    );

    lcd_instruction(LCD_INSTRUCTION_DISPLAY_ON);  // Turn on the display
    lcd_instruction(LCD_INSTRUCTION_RETURN_HOME);  // Return to address 0 ("home")
    lcd_instruction(LCD_INSTRUCTION_CLEAR_DISPLAY);  // Clear the display of any existing characters

    __delay_cycles(50000);
}

/**
 * Write a string to the specified line of the display. Note that only the first 16 characters of the string will be
 * written, as this is the horizontal capacity of the LCD.
 */
void lcd_display_string(int line, char *string) {
    // Ignore the request if a threshold update speed has been imposed on the LCD, and the threshold has not been met.
    if (wait_cycles++ < cycle_threshold) {
        return;
    }
    wait_cycles = 0;

    if (line == LCD_LINE_2) {
        // Because of some unknown weirdness, it's not possible to write to LCD_LINE_2 if LCD_LINE_1 hasn't been
        // written to yet. Simply setting the line doesn't effect the external behavior of the display, so a
        // workaround is to set the line to LCD_LINE_1 on every LCD_LINE_2 request, followed by setting it to
        // LCD_LINE_2 as normal. Of course, this workaround isn't necssary to write to LCD_LINE_1.
        lcd_set_line(LCD_LINE_1);
    }
    lcd_set_line(line);

    int i;
    for (i = 0; i < min(strlen(string), 16); i++) {
        lcd_display_char(string[i]);
    }

    // Pad the remaining screen space with spaces.
    while (i++ < 16) {
        lcd_display_char(' ');
    }
}

/**
 * Clear the entire display.
 */
void lcd_clear_all() {
    lcd_instruction(LCD_INSTRUCTION_CLEAR_DISPLAY);
}

/**
 * Clear the specified line of the LCD.
 */
void lcd_clear_line(int line) {
    if (line == LCD_LINE_1 || line == LCD_LINE_2) {
        // Is there a less embarassing way to do this? Probably.
        lcd_display_string(line, "                ");
    }
}

/**
 * Send 4 bits of data to the LCD.
 */
static void lcd_tx_4_bit(char data) {
    LCD_PORT_OUT |= LCD_EN;  // Indicate that we're able to write data
    LCD_PORT_OUT |= data << LCD_DATA_START_PIN;  // Offset to the correct start pin
    LCD_PORT_OUT &= ~LCD_EN;  // Indicate that we're no longer writing data
    LCD_PORT_OUT &= ~LCD_DATA;  // Pull the output to low; we're done writing this data
}

/**
 * Send 8 bits of data to the LCD as two 4-bit transmissions.
 */
static void lcd_tx_8_bit(char data) {
    // Separately transmit the lower and upper halves of the byte.
    // This 8-bit-as-4-bit-transmission scheme is technically twice as slow, but we also use half as many IO pins.
    lcd_tx_4_bit(data >> 4);
    lcd_tx_4_bit(data & 0x0F);
}

/**
 * Set the line (one of LCD_LINE_1 or LCD_LINE_2) that characters should be written to. Note that the behavior of this
 * function is stateful; it should not be externally called! Instead, the lcd_display_string function serves as a
 * stateless wrapper for this logic.
 */
static void lcd_set_line(int line) {
    // It might appear that some black magic is going on here, and you are probably correct.
    // This essentially positions the LCD cursor to either line 1 (0x00) or 2 (0x40) and column 0 (0x80).
    lcd_instruction((line == LCD_LINE_1 ? 0 : 0x40) | 0x80);
}

/**
 * Write a single character to the next available position in the display.
 */
static void lcd_display_char(char c) {
    __delay_cycles(TRANSMISSION_DELAY);
    LCD_PORT_OUT |= LCD_RS;  // Set the RS bit high to indicate we're about to write a character
    lcd_tx_8_bit(c);
}

/**
 * Execute a special control (non-character write) instruction, as defined by one of the LCD_INSTRUCTION_x macros.
 */
static void lcd_instruction(char instruction) {
    __delay_cycles(TRANSMISSION_DELAY);
    LCD_PORT_OUT &= ~LCD_RS;  // Set the RS bit low to indicate we're about to send a special instruction
    lcd_tx_8_bit(instruction);
}
