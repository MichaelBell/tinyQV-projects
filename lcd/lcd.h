#pragma once
#include <stdint.h>

void setup_lcd();
void lcd_clear_screen(uint16_t colour);
void lcd_draw_sprite(int x, int y, int w, uint16_t* data, int len);