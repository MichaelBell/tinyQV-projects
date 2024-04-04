#pragma once
#include <stdint.h>

void setup_lcd();
void lcd_clear_screen(uint16_t colour);
void lcd_draw_sprite(int x, int y, int w, const uint16_t* data, int len);
void lcd_draw_sprite_portion(int x, int y, int sprite_w, const uint16_t* data, int offset_x, int offset_y, int w, int h);