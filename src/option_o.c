#include <stdio.h>

void handle_option_o(FILE *f, const char *output_file)
{
    FILE *out = fopen(output_file, "w");
    if (!out)
    {
        perror("Error opening output file");
        return;
    }

    char ch;
    while ((ch = fgetc(f)) != EOF)
    {
        fputc(ch, out); // 원본 파일을 그대로 저장
    }

    fclose(out);
    printf("File saved to %s\n", output_file);
}
