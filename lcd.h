/**
 * Defined constants and function prototypes used for controlling the LCD.
 * This LCD_PORT macros and LCD pin definitions can be reassigned appropriately to match the wiring configuration.
 */


/* Define the port the LCD is connected to; defaults to Port 2. */
#define LCD_PORT_OUT P2OUT
#define LCD_PORT_DIR P2DIR

/* Define aliases for the pins of the LCD. */
#define LCD_RS BIT0  // RS on P2.0
#define LCD_EN BIT1  // EN on P2.1
#define LCD_DATA (BIT2 | BIT3 | BIT4 | BIT5)  // DB4 on P2.2, DB5 on P2.3, DB6 on P2.4, DB7 on P2.5
#define LCD_DATA_START_PIN 2  // The P2.x pin at which the data pins start. By default, it starts at P2.2 (and continues to P2.5).

/* Define encodings for the two lines available on the LCD. */
#define LCD_LINE_1 0
#define LCD_LINE_2 1

/**
 * Initialize the LCD. This function must be called before any other LCD control functions. This behavior of this
 * function is not idempotent; calling it multiple times will have the effect of resetting anything currently
 * displayed on the screen.
 */
void lcd_init(int cycle_delay_threshold);

/**
 * Write a string to the specified line of the display. Note that only the first 16 characters of the string will be
 * written, as this is the horizontal capacity of the LCD.
 */
void lcd_display_string(int line, char *string);

/**
 * Clear the entire display.
 */
void lcd_clear_all();

/**
 * Clear the specified line of the LCD.
 */
void lcd_clear_line(int line);
