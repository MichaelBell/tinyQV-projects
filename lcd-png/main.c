#include <stdio.h>

#include <pngdec/PNGdec.h>
#include <fatfs/ff.h>
#include <gpio.h>
#include <spi.h>

#include "lcd.h"

FATFS fs;
FIL fil;
FILINFO filinfo;
DIR dir;
FRESULT fr;

PNGIMAGE png;

int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  UINT read_len;
  if (f_read(&fil, buffer, (UINT)length, &read_len) != FR_OK) {
    printf("Read failed\n");
    //__breakpoint();
  }
  printf("Read %d of %ld bytes: ", read_len, length);
  printf("%02x %02x %02x %02x .. ", buffer[0], buffer[1], buffer[2], buffer[3]);
  printf("%02x %02x %02x %02x\n", buffer[read_len-4], buffer[read_len-3], buffer[read_len-2], buffer[read_len-1]);
  return (int32_t)read_len;
}
int32_t mySeek(PNGFILE *handle, int32_t position) {
  if (f_lseek(&fil, position) != FR_OK) {
    printf("Seek failed\n");
    //__breakpoint();
  }
  printf("Seek to %ld\n", position);
  return position;
}

static inline void lcd_deselect() {
    //printf("Deselect: %02lx\n", get_outputs());
    while ((get_outputs() & 0b00010000) == 0);
    set_gpio_sel(0b10010100);
    //spi_set_config(0);
}

static inline void lcd_select() {
    set_gpio_sel(0b10000000);
    //printf("Select: %02lx\n", get_outputs());
    //spi_set_config(0);
}

// Function to draw pixels to the display
void PNGDraw(PNGDRAW *pDraw) {
  uint16_t usPixels[320];

  //printf("Draw, y = %d, iWidth = %d: ", pDraw->y, pDraw->iWidth);

  PNGRGB565(pDraw, usPixels, PNG_RGB565_BIG_ENDIAN, 0xffffffff, png.iHasAlpha);

  //printf("%04x %04x %04x %04x\n", usPixels[0], usPixels[1], usPixels[2], usPixels[3]);

  lcd_select();
  lcd_draw_rect(0, pDraw->y, pDraw->iWidth, usPixels, pDraw->iWidth);
  lcd_deselect();
}

int main() {
    memset(&png, 0, sizeof(PNGIMAGE));
    png.pfnRead = myRead;
    png.pfnSeek = mySeek;
    png.pfnDraw = PNGDraw;

    set_outputs (0b10010000);  // Set both CS on

    lcd_select();
    setup_lcd();
    lcd_deselect();

    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
      printf("Failed to mount SD card, error: %d\n", fr);
      return 0;
    }
    printf("Mounted SD card\n");

    f_opendir(&dir, "/");
    while(f_readdir(&dir, &filinfo) == FR_OK && filinfo.fname[0]) {
        printf("- %s %lld\n", filinfo.fname, filinfo.fsize);
        const int len = strlen(filinfo.fname);
        if (len > 3 && strcmp(filinfo.fname + len - 3, "png") == 0) {
            printf("Draw %s\n", filinfo.fname);
            if (f_open(&fil, filinfo.fname, FA_READ) != FR_OK) {
                printf("Error opening file\n");
                return -1;
            }
            png.PNGFile.iSize = filinfo.fsize;
            png.PNGFile.fHandle = &fil;

            int rv = PNGInit(&png);
            printf("Initialized: %d\n", rv);
            rv = DecodePNG(&png, NULL, 0);
            printf("Decoded: %d\n", rv);

            if (f_close(&fil) != FR_OK) {
                printf("Error closing file\n");
                return -1;                
            }
            printf("Done\n");
        }
    }
    f_closedir(&dir);    

    return 0;
}