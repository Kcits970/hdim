#include "change_endian.h"

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
    unsigned char buf[4];
    size_t read;

    while ((read = fread(buf, 1, word_size, f)) == word_size) {
        if (word_size == 2) {
            uint16_t val = buf[0] | (buf[1] << 8);
            if (to_big_endian) val = swap16(val);
            printf("%04x ", val);
        } else if (word_size == 4) {
            uint32_t val = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
            if (to_big_endian) val = swap32(val);
            printf("%08x ", val);
        }
    }

    printf("\n");
}

// 사용 예시:
// FILE *f = fopen("binary.bin", "rb");
// print_as_endian(f, 2, 1); // 2바이트 단위, 빅엔디안 출력
// print_as_endian(f, 4, 0); // 4바이트 단위, 리틀엔디안 출력
// fclose(f);
