#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void) {
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  while (SCANLINECOUNTER > 160);

  // Write a while loop that keeps going until we're in vBlank:
  while (SCANLINECOUNTER < 160);

  // Finally, increment vBlank:
  vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color) {
  videoBuffer[OFFSET(row, col, WIDTH)] = color; // equivalent to *(videoBuffer + OFFSET() = color), setting the pointer's value
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  // test if we actually need this statement below later
  DMA[DMA_CHANNEL_3].cnt = 0; // prevent DMA from drawing before we set up src and dst 
  // We use DMA_CHANNEL_3 because that is the one for general purpose copies of memory
  DMA[DMA_CHANNEL_3].src = (const void *) &color; // color holds the address of the color we want, which is why it's volatile bc the compiler might never initalize the variable otherwise
  for (int i = 0; i < height; i++) { 
    // & is needed in the below expression because videoBuffer alone means a pointer, videoBuffer[x] is equivalent to *(videoBuffer + x) which is a value
    DMA[DMA_CHANNEL_3].dst = &videoBuffer[OFFSET(row + i, col, WIDTH)]; // filling in the current row
    DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON; // ORing a bunch of things we want for the control regsiter
  }
}

void drawFullScreenImageDMA(const u16 *image) {
  DMA[DMA_CHANNEL_3].cnt = 0; // prevent DMA from drawing before we set up src and dst 
  DMA[DMA_CHANNEL_3].src = image;
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = WIDTH*HEIGHT | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
}

// Nearly identical to the logic of drawRectDMA
void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  DMA[DMA_CHANNEL_3].cnt = 0; // prevent DMA from drawing before we set up src and dst 
  DMA[DMA_CHANNEL_3].src = image;
  for (int i = 0; i < height; i++) {
    DMA[DMA_CHANNEL_3].dst = &videoBuffer[OFFSET(row + i, col, WIDTH)];
    DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
  }
}

void undrawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // Used to remove the trails left behind of previous images of a moving object/sprite
  // Not needed because we're going to have a background color, so we can just use the drawRectDMA method instead
  UNUSED(row);
  UNUSED(col);
  UNUSED(width);
  UNUSED(height);
  UNUSED(image);
}

void fillScreenDMA(volatile u16 color) {
  DMA[DMA_CHANNEL_3].cnt = 0; // prevent DMA from drawing before we set up src and dst 
  DMA[DMA_CHANNEL_3].src = &color; 
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = WIDTH*HEIGHT | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON; // WIDTH*HEIGHT indicates the number of pixels/indices we want to fill
}

void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}
