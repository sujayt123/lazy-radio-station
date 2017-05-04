# Lazy Man's Radio

You’ve just come home from your 12 hour shift at Kemere Labs. It's time to lay back and destress with some of your favorite tunes. To your dismay, you find that changing songs requires an inordinate amount of effort. Who has time to reach over for the darned skip button every time she wants to move on to the next song?

That’s where we come in!

Presenting, the lazy man’s radio. An inexpensive and modern alternative to today’s music players. Simply wave your hand in front of the sensor to trigger a “skip” to the next song! Our music player features all the standard capabilities of its contemporaries for a fraction of the price.

### [Demo](https://www.youtube.com/watch?v=JdD40mU05lw)

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

### [Circuit](https://sujayt123.github.io/lazy-radio-station/circuit.jpeg)


### Architecture
##### Hardware

##### Software

### Discussion

### [Code](github.com/sujayt123)
