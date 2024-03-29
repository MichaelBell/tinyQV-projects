#include "lcd.h"
#include "font.h"
#include <nanoprintf.h>
#include <string.h>

#define FONT (&ubuntu_mono8)
#define BG_COLOUR 0

static uint32_t font_colours[16];

static uint32_t* add_char_line(uint32_t* buf, int c, int y) {
    const lv_font_fmt_txt_glyph_dsc_t* g = &FONT->dsc->glyph_dsc[c - 0x20 + 1];
    const uint8_t *b = FONT->dsc->glyph_bitmap + g->bitmap_index;
    const int ey = y - FONT_HEIGHT + 4 + g->ofs_y + g->box_h;
    if (ey < 0 || ey >= g->box_h || g->box_w == 0) {
        for (int i = 0; i < 4; ++i) {
            *buf++ = BG_COLOUR | (BG_COLOUR << 16);
        }
    }
    else {
        int bi = (g->box_w * ey);

        uint32_t bits = (b[bi >> 2] << 16) | (b[(bi >> 2) + 1] << 8) | b[(bi >> 2) + 2];
        bits >>= 8 - ((bi & 3) << 1);
        bits &= 0xffff & (0xffff << ((8 - g->box_w) << 1));
        int32_t ofs_x = g->ofs_x;
        if (g->box_w < 6) --ofs_x;
        bits >>= ofs_x << 1;

        *buf++ = font_colours[bits >> 12];
        *buf++ = font_colours[(bits >> 8) & 0xf];
        *buf++ = font_colours[(bits >> 4) & 0xf];
        *buf++ = font_colours[bits & 0xf];
    }

    return buf;
}

static void build_font_colours() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            font_colours[i*4 + j] = __builtin_bswap16((0x0841 * i * 10) + BG_COLOUR) | (__builtin_bswap16((0x0841 * j * 10) + BG_COLOUR) << 16);
        }
    }
}

void lcd_text_init() {
    build_font_colours();
}

uint32_t glyph_buf[64];

int lcd_display_char(int c, int x, int y)
{
    uint32_t* buf = glyph_buf;
    for (int i = 0; i < FONT_HEIGHT; ++i)
    {
        buf = add_char_line(buf, c, i);
    }
    lcd_draw_sprite(x, y, 8, (uint16_t*)glyph_buf, 128);
    if (FONT->dsc->glyph_dsc[c - 0x20 + 1].box_w < 6) return 6;
    else return 8;
}

void lcd_display_string(const char* str, int x, int y)
{
    int start_x = x;
    while (*str) {
        if (*str == '\n') { x = start_x; y += 20; }
        else if (*str == ' ') { 
            memset(glyph_buf, 0, 80*2);
            lcd_draw_sprite(x, y, 5, (uint16_t*)glyph_buf, 80);
            x += 5;
        }
        else {
            x += lcd_display_char(*str, x, y);
        }
        ++str;
    }
}

int lcd_printf(int x, int y, const char* fmt, ...) {
    char buf[128];
    va_list val;
    va_start(val, fmt);
    int const rv = npf_vsnprintf(buf, 128, fmt, val);
    va_end(val);
    lcd_display_string(buf, x, y);
    return rv;
}
