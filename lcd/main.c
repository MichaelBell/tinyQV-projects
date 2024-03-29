#include "lcd.h"
#include <gpio.h>
#include <uart.h>
#define printf uart_printf

#include "lcd-text.h"

#include "font.h"

#if 0
struct sprite
{
    uint16_t* data;
    int len;
    int width;
};
#endif

struct ball
{
    int x;
    int y;
    int xvel;
    int yvel;
};

const uint16_t ball_sprite_data[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x00F0, 0x00F0, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x0000, 0x0000,
    0x0000, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x0000,
    0x0000, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x0000,
    0x0000, 0x0000, 0x00F0, 0x00F0, 0x00F0, 0x00F0, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x00F0, 0x00F0, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

uint16_t sprite_buf[64];

static void delay_loop(int cycles) {
    volatile int i;
    for (i = 0; i < cycles; ++i);
}

static void move_ball(int old_x, int old_y, int inc_x, int inc_y)
{
    int top_x = inc_x > 0 ? 1 : 0;
    int top_y = inc_y > 0 ? 1 : 0;

    for (int y = 0; y < 7; ++y) {
        for (int x = 0; x < 7; ++x) {
            sprite_buf[7 * y + x] = ball_sprite_data[8 * (y - top_y + 1) + (x - top_x + 1)];
        }
    }
    lcd_draw_sprite(old_x, old_y, 7, sprite_buf, 7*7);
}

#define BAT_Y 235
#define BAT_COLOUR 7
#define BAT_WIDTH 32
static void draw_bat(int x)
{
    for (int i = 0; i < BAT_WIDTH*2; ++i) {
        sprite_buf[i] = BAT_COLOUR;
    }
    lcd_draw_sprite(x, BAT_Y, BAT_WIDTH, sprite_buf, BAT_WIDTH*2);
}

static void move_bat(int old_x, int inc_x)
{
    sprite_buf[0] = 0;
    sprite_buf[1] = 0;
    sprite_buf[2] = BAT_COLOUR;
    sprite_buf[3] = BAT_COLOUR;

    if (inc_x > 0) {
        lcd_draw_sprite(old_x, BAT_Y, 1, sprite_buf, 2);
        lcd_draw_sprite(old_x + BAT_WIDTH, BAT_Y, 1, sprite_buf+2, 2);
    }
    else if (inc_x < 0) {
        lcd_draw_sprite(old_x - 1, BAT_Y, 1, sprite_buf+2, 2);
        lcd_draw_sprite(old_x + BAT_WIDTH - 1, BAT_Y, 1, sprite_buf, 2);
    }
    else {
        // Do nothing at similar cost
        lcd_draw_sprite(old_x, BAT_Y, 1, sprite_buf+2, 2);
        lcd_draw_sprite(old_x, BAT_Y, 1, sprite_buf+2, 2);
    }
}

static void run_game() {
    lcd_clear_screen(0);

    int bat_x = 110;
    draw_bat(bat_x);

    struct ball b = {.x=50<<4, .y=200<<4, .xvel=1, .yvel=-1<<2};

    int frame = 0;

    while (1) {
        if ((++frame & 0xFFF) == 0) {
            if (b.yvel < 0) --b.yvel;
            if (b.yvel > 0) ++b.yvel;
        }
        if ((frame & 0xFF) == 0) {
            sprite_buf[0] = 0xFFFF;
            lcd_draw_sprite(frame >> 8, 0, 1, sprite_buf, 1);
        }

        const int inputs = ~get_inputs();
        //uart_putc('.');
        //printf("%08x\n", inputs);

        const int old_ball_x = b.x >> 4;
        const int old_ball_y = b.y >> 4;

        b.x += b.xvel;
        b.y += b.yvel;
        if ((b.x < 1<<4 && b.xvel < 0) || (b.x > 231<<4 && b.xvel > 0)) b.xvel = -b.xvel;
        if (b.y < 2<<4) b.yvel = -b.yvel;

        const int new_ball_x = b.x >> 4;
        if (b.y > (BAT_Y - 8) << 4) {
            if (new_ball_x + 5 > bat_x && new_ball_x < bat_x + BAT_WIDTH) {
                b.yvel = -b.yvel;
                if (inputs & 1) {
                    --b.xvel;
                    if (b.xvel == 0) b.xvel = -1;
                }
                else if (inputs & 2) {
                    ++b.xvel;
                    if (b.xvel == 0) b.xvel = 1;
                }
            }
            else break;
        }

        const int new_ball_y = b.y >> 4;
        //uart_putc('.');

        // Call move ball even if not moved otherwise timing will chnage a lot
        move_ball(old_ball_x, old_ball_y, new_ball_x - old_ball_x, new_ball_y - old_ball_y);

        if ((frame & 3) == 0) {
            if ((inputs & 1) && bat_x > 1) {
                move_bat(bat_x, -1);
                bat_x--;
            }
            else if ((inputs & 2) && bat_x < 238 - BAT_WIDTH) {
                move_bat(bat_x, 1);
                bat_x++;
            }
            else {
                move_bat(bat_x, 0);
            }
        }

        //uart_putc('.');
    }

    printf("Game Over\r\n");
    lcd_display_string("Game Over!", 80, 40);
}

int main(void)
{
    printf("Start\r\n");
    setup_lcd();
    printf("LCD setup\r\n");
    lcd_text_init();

    lcd_display_string("!Hello, world!?!", 10, 10);
    printf("Hello\r\n");
    lcd_display_string("TinyQV RV32EC system\nMike's submission for\nTinyTapeout 6", 10, 50);

    lcd_printf(10, 150, "Printf test: %d %02x", 23, 67);
    printf("Test\r\n");

    delay_loop(1000);

    while (1) {
        run_game();
        delay_loop(500);
        while ((~get_inputs() & 3) == 0);
    }
}
