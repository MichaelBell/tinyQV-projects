#include "fatfs/ff.h"
#include <stdio.h>

FATFS fs;
FIL fil;
FILINFO filinfo;
DIR dir;
FRESULT fr;

int main(void) {
    printf("Hello\n");

    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
      printf("Failed to mount SD card, error: %d\n", fr);
      return 0;
    }
    printf("Mounted SD card\n");

    f_opendir(&dir, "/");
    while(f_readdir(&dir, &filinfo) == FR_OK && filinfo.fname[0]) {
        printf("- %s %lld\n", filinfo.fname, filinfo.fsize);
    }
    f_closedir(&dir);

    return 0;
}
