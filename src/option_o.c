#include <stdio.h>

void handle_option_o(FILE *f)
{
    unsigned char buf[2];

    while (fread(buf, sizeof(unsigned char), 2, f) == 2)
    {
        unsigned int value = (buf[0] << 8) | buf[1];
        printf("%06o ", value); // 6자리 8진수 출력 (앞에 0 포함)
    }
    printf("\n");
}
