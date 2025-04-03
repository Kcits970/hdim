#include <stdio.h>

void handle_option_d(FILE *f)
{
    unsigned char buf[2];

    while (fread(buf, sizeof(unsigned char), 2, f) == 2)
    {
        printf("%05u ", (buf[0] << 8) | buf[1]); // 2바이트를 10진수로 변환하여 출력
    }
    printf("\n");
}
