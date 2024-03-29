#include <spi.h>
#include <gpio.h>
#include <uart.h>
#define printf uart_printf

#define LCD_CS 0
#define LCD_DC 3
#define LCD_SCK 1
#define LCD_MOSI 2
#define LCD_BL 4

#define REP4(X) X X X X
#define REP16(X) REP4(REP4(X))

// Format: cmd length (including cmd byte), post delay in units of 5 ms, then cmd payload
// Note the delays have been shortened a little
static const uint8_t st7789_init_seq[] = {
        1, 20, 0x01,                         // Software reset
        1, 10, 0x11,                         // Exit sleep mode
        2, 2, 0x3a, 0x55,                   // Set colour mode to 16 bit
        2, 0, 0x36, 0x00,                   // Set MADCTL: row then column, refresh is bottom to top ????
        1, 2, 0x21,                         // Inversion on, then 10 ms delay (supposedly a hack?)
        1, 2, 0x13,                         // Normal display on, then 10 ms delay
        1, 2, 0x29,                         // Main screen turn on, then wait 500 ms
        0                                     // Terminate list
};

static void lcd_write_cmd(const uint8_t* cmd, int len) {
    if (len == 1) spi_send_data(cmd[0] | 0x100);
    else {
        spi_send_data(cmd[0]);
        spi_send_bytes(cmd+1, len-1, true, true);
    }
}

static void delay_loop(int cycles) {
    volatile int i;
    for (i = 0; i < cycles; ++i);
}

static void start_pixels(int x, int y, int w) {
    uint8_t cmd[5];
    cmd[0] = 0x2A;
    cmd[1] = 0;
    cmd[2] = x;
    cmd[3] = 0;
    cmd[4] = x+w-1;
    lcd_write_cmd(cmd, 5);

    cmd[0] = 0x2B;
    cmd[2] = y;
    cmd[4] = 240;
    lcd_write_cmd(cmd, 5);

    spi_send_data(0x2C);
}

void setup_lcd()
{
    spi_set_divider(0);
    
    const uint8_t *cmd = st7789_init_seq;
    while (*cmd) {
        lcd_write_cmd(cmd + 2, *cmd);
        delay_loop(*(cmd + 1) * 150);
        cmd += *cmd + 2;
    }

    start_pixels(0, 0, 240);
    for (int i = 0; i < 240*240-1; ++i) {
        spi_send_data(0x200);
        spi_send_data(0x200);
    }
    spi_send_data(0x200);
    spi_send_data(0x300);
}

void lcd_clear_screen(uint16_t colour)
{
    start_pixels(0, 0, 240);
    uint8_t data[2];
    data[0] = colour >> 8;
    data[1] = colour & 0xFF;
    for (int i = 0; i < 240*240-1; ++i) {
        spi_send_data(data[0] | 0x200);
        spi_send_data(data[1] | 0x200);
    }
    spi_send_data(data[0] | 0x200);
    spi_send_data(data[1] | 0x300);
}

void lcd_draw_sprite(int x, int y, int w, uint16_t* data, int len)
{
    start_pixels(x, y, w);
    spi_send_bytes((uint8_t*)data, len*2, true, true);
}