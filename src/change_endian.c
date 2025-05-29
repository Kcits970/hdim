/*
기능 안쓴다함
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t swap16(uint16_t val) {
    return (val >> 8) | (val << 8);
}

uint32_t swap32(uint32_t val) {
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

void print_as_endian(FILE *f, int word_size, int to_big_endian) {
    unsigned char buf[1024];
    size_t count;
    unsigned int row = 0;

    while ((count = fread(buf, sizeof(unsigned char), sizeof(buf), f)) > 0) {
        if (count & 1)
            buf[count++] = 0;  // 홀수 바이트라면 패딩

        unsigned int off = 0;
        while (off < count) {
            fprintf(stdout, "%08x ", row);
            for (size_t i = 0; i < 0x10 && off + i + word_size - 1 < count; i += word_size) {
                if (word_size == 2) {
                    uint16_t val = buf[off + i] | (buf[off + i + 1] << 8);
                    if (to_big_endian) val = swap16(val);
                    fprintf(stdout, "%02x%02x ", (val >> 8) & 0xFF, val & 0xFF);
                } else if (word_size == 4) {
                    uint32_t val = buf[off + i] | (buf[off + i + 1] << 8) |
                                    (buf[off + i + 2] << 16) | (buf[off + i + 3] << 24);
                    if (to_big_endian) val = swap32(val);
                    fprintf(stdout, "%02x%02x%02x%02x ",
                            (val >> 24) & 0xFF,
                            (val >> 16) & 0xFF,
                            (val >> 8) & 0xFF,
                            val & 0xFF);
                }
            }
            fprintf(stdout, "\n");
            row += 0x10;
            off += 0x10;
        }
    }
}
