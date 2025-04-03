#include <stdio.h>

void handle_option_c(FILE *f)
{
    char ch;
    while ((ch = fgetc(f)) != EOF)
    {
        putchar(ch);  // 문자 그대로 출력
    }
    printf("\n");
}
