#tcmp441 library
This is a library for the TCM-P441-230_v1.0 module from pervasive displays. This library provides an api to write text, images, pixels, and qr codes to the eink display.

##API
```int x``` and ```int y``` are 0-400 and 0-300 respectively, unless otherwise stated.

* ```void tcmp441_init(int led0, int led1, int led2, int ntc_en, int ntc_busy, int ntc_cs)```
Sets up the gpio pins and spi connection to the display. leds 0-2 are just indicators. 

    ```ntc_en``` is the spi enable pin

    ```ntc_busy``` is the busy pin

    ```ntc_cs``` is the chip select pin

    Example
    ```void tcmp441_init(18, 19, 20, 24, 23, 22);```

* ```void tcmp441_updateDisplay()```
Updates the display. It applies all of the changes made by most of the other functions.

* ```void tcmp441_clearScreen()```
This function is very self explanatory. It clears the screen. You still need to call ```updateDisplay()``` to apply the change, though.

* ```void tcmp441_setPixel(int x, int y, int on)```
Sets a pixel on the display. ```int on``` is either 0 or 1.

* ```void tcmp441_insertPixelGrid(int width, int height, uint8_t grid[height][width], int xcoord, int ycoord)```
Inserts a grid of pixels. ```(xcoord, ycoord)``` is the coordinate of the upper left corner. Each element of the grid is either 0 or 1.

* ```void tcmp441_setBlock(int x, int y, int on)```
Sets a block of pixels 8x8 to on (1) or off (0). The 400x300 display has been split up into 50x37 chunks. Your x and y values should be between those respective values.

* ```void tcmp441_insertBigPixelGrid(int width, int height, uint8_t grid[height][width], int xcoord, int ycoord)```
This function is a combination of setBlock and insertPixelGrid.

* ```void tcmp441_writeCharacterAtLocation(char character, int xcoord, int ycoord, uint8_t scale)```
Writes a character at a given location with a given scale.

* ```void tcmp441_writeStringAtLocation(char *str, int x, int y, int scale)```
Writes a string of characters.

* ```void tcmp441_writeQRcode(char *str)```
Writes a qr code using setBlock and starting in the upper left corner.