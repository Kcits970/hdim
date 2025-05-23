#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dump.h"
#include "options.h"

int main(int argc, char **argv)
{
    int dump_mode = 0, char_mode = 0, version_mode = 0;
    char *input_file = NULL;
    char *output_file = NULL;

    // 옵션 및 입력 파일 분석
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--d") == 0) {
            dump_mode = 1;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--c") == 0) {
            char_mode = 1;
        } else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--V") == 0) {
            version_mode = 1;
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing output file after -o option.\n");
                return 1;
            }
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            return 1;
        } else if (input_file == NULL) {
            input_file = argv[i]; // 옵션이 아닌 첫 번째 인자를 입력 파일로 간주
        } else {
            fprintf(stderr, "Warning: Extra argument '%s' is ignored.\n", argv[i]);
        }
    }

    // 입력 파일 필수
    if (input_file == NULL) {
        fprintf(stderr, "Usage: ./hdim [-d] [-c] [-V] [-o output_file] <file>\n");
        return 1;
    }

    // 파일 열기
    FILE *f = fopen(input_file, "r");
    if (!f) {
        fprintf(stderr, "./hdim: ");
        perror(input_file);
        return 1;
    }

    // 각 옵션 실행
    if (dump_mode)
        handle_option_d(f);
    if (char_mode)
        handle_option_c(f);
    if (version_mode)
        handle_option_v();
    if (output_file)
        handle_option_o(f, output_file);

    fclose(f);
    return 0;
}
