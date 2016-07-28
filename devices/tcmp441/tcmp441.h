#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void tcmp441_clearScreen();

void tcmp441_init(int led0, int led1, int led2, int ntc_en, int ntc_busy, int ntc_cs);

void tcmp441_updateDisplay();

//on = 1 or 0
void tcmp441_setPixel(int x, int y, int on);

void tcmp441_insertPixelGrid(int width, int height, uint8_t grid[height][width], int xcoord, int ycoord);
void tcmp441_setBlock(int x, int y, int on);
void tcmp441_insertBigPixelGrid(int width, int height, uint8_t grid[height][width], int xcoord, int ycoord);

void tcmp441_writeCharacterAtLocation(char character, int xcoord, int ycoord, uint8_t scale);
void tcmp441_writeStringAtLocation(char *str, int x, int y, int scale);

void tcmp441_writeQRcode(char *str);

