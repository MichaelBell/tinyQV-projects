#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define RULER_LEN 29
#define RULER_MARKS 8

bool check(uint32_t r)
{
    r |= (1 << (32 - RULER_LEN));
    uint32_t v = r;
    do {
        while ((r & 0x80000000) == 0) {
            r <<= 1;
        }
        v |= r << 1;
        r <<= 1;
        //printf("Check: %08x %08x\n", v, ~0 << (32 - RULER_LEN));
    } while (r);
    return v == ~0 << (32 - RULER_LEN);
}

uint32_t step(uint32_t r, uint32_t m)
{
    uint32_t new_r = r;
    uint32_t last_r = 1 << (32 - RULER_LEN + m);

    // Strip m marks.
    for (int i = 0; i < m; ++i) {
        new_r &= new_r-1;
    }
    
    // Increment last mark
    uint32_t mark = new_r & (-new_r);
    new_r ^= mark;
    mark >>= 1;

    // Return 0 if this mark hit the next one
    if (mark == last_r) return 0;

    for (int i = 0; i <= m; ++i) {
        new_r |= mark;
        mark >>= 1;
    }
    
    return new_r;
}

uint32_t find_ruler()
{
    uint32_t r = ~0 << (32 - RULER_MARKS + 1);
    
    while (true) {
        uint32_t new_r = step(r, 0);
        uint32_t m = 1;
        while (new_r == 0 && m < RULER_MARKS - 1) {
            new_r = step(r, m++);
        }
        if (new_r == 0) break;
        //printf("%08x %08x %d\n", new_r, r, m);
        r = new_r;
        if (check(r)) return r;
    }

    return 0;
}

int main(void) {
    uint32_t r = find_ruler();
    printf("0 ");
    for (int i = 1; i < RULER_LEN; ++i) {
        if (r & (1 << (32 - i))) printf("%d ", i);
    }
    printf("%d\n", RULER_LEN);
    return 0;
}