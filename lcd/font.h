/*
 * Modified from a Pico example:
 *
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <stdint.h>

typedef struct {
    uint16_t bitmap_index;
    uint16_t adv_w;
    int8_t box_w, box_h, ofs_x, ofs_y;
} __attribute__((packed)) lv_font_fmt_txt_glyph_dsc_t;

typedef struct {
    uint16_t range_start, range_length, glyph_id_start, list_length;
    void *unicode_list, *glyph_id_ofs_list;
    enum {
        LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    } type;
} lv_font_fmt_txt_cmap_t;

typedef struct {
    const uint8_t *glyph_bitmap;
    const lv_font_fmt_txt_glyph_dsc_t *glyph_dsc;
    const lv_font_fmt_txt_cmap_t *cmaps;
    uint8_t cmap_num, bpp, kern_scale, kern_classes;
    void *kern_dsc;
} lv_font_fmt_txt_dsc_t;

typedef struct {
    lv_font_fmt_txt_dsc_t *dsc;
    uint8_t line_height, base_line;
} lv_font_t;

#define FONT_WIDTH_WORDS 4
#define FONT_HEIGHT 16
#define FONT_SIZE_WORDS (FONT_HEIGHT * FONT_WIDTH_WORDS)
extern const lv_font_t ubuntu_mono8;
