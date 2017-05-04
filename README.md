# msp430-hd44780u

A simple, no-BS library for interfacing the MSP430 with the [HD44780U LCD display](https://www.adafruit.com/products/181)


### Configuration

You'll likely need to change the following `#define` statements in `lcd.h` to match your hardware configuration.

```c
/* Define the port the LCD is connected to; defaults to Port 2. */
#define LCD_PORT_OUT P2OUT
#define LCD_PORT_DIR P2DIR

/* Define aliases for the pins of the LCD. */
#define LCD_RS BIT0  // RS on P2.0
#define LCD_EN BIT1  // EN on P2.1
#define LCD_DATA (BIT2 | BIT3 | BIT4 | BIT5)  // DB4 on P2.2, DB5 on P2.3, DB6 on P2.4, DB7 on P2.5
#define LCD_DATA_START_PIN 2  // The P2.x pin at which the data pins start. By default, it starts at P2.2 (and continues to P2.5).
```

### Usage

Include the `lcd.h` header file in your source.

```c
// main.c

#include "lcd.h"

...

int main() {
	lcd_init(N);  // See the documentation in `lcd.h` on the parameter value
    
    ...
}
```

### Compilation

Add `lcd.c` as one of the source files passed to `msp430-gcc`.

```bash
msp430-gcc main.c lcd.c
```

### Additional Documentation

Please see `lcd.c` for more comprehensive documentation of the library's functionality.
