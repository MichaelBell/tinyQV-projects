#pragma once

void lcd_text_init();
int lcd_display_char(int c, int x, int y);
void lcd_display_string(const char* str, int x, int y);
void lcd_printf(int x, int y, const char* fmt, ...);