#pragma once
#include <stdint.h>

void setup_lcd();
void lcd_clear_screen(uint16_t colour);
void lcd_draw_rect(int x, int y, int w, const uint16_t* data, int len);
