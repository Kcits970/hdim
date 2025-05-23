#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "dump.h"
#include "options.h"

int main(int argc, char **argv)
{
    // 각 기능별 플래그 변수
    int dump_mode = 0, char_mode = 0, version_mode = 0, octal_mode = 0;
    char *input_file = NULL;

    // 인자 파싱 (옵션 순서 무관, -- 및 소문자 지원)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--d") == 0) {
            dump_mode = 1;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--c") == 0) {
            char_mode = 1;
        } else if (
            strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--V") == 0 ||
            strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--v") == 0
        ) {
            version_mode = 1;
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--o") == 0) {
            octal_mode = 1;
        } else if (argv[i][0] == '-') {
            // 정의되지 않은 옵션
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            return 1;
        } else if (input_file == NULL) {
            // 첫 번째 옵션이 아닌 인자를 입력 파일로 간주
            input_file = argv[i];
        } else {
            // 그 외 불필요한 인자는 경고만 출력
            fprintf(stderr, "Warning: Extra argument '%s' is ignored.\n", argv[i]);
        }
    }

    // 입력 파일이 없을 경우 사용법 안내
    if (input_file == NULL) {
        fprintf(stderr, "Usage: ./hdim [-d] [-c] [-V|-v] [-o] <file>\n");
        return 1;
    }

    // 각 옵션별로 파일 새로 열고 닫기
    if (dump_mode) {
        FILE *f = fopen(input_file, "r");
        if (f) {
            handle_option_d(f);
            fclose(f);
        } else {
            perror("Error opening file for -d");
        }
    }

    if (char_mode) {
        FILE *f = fopen(input_file, "r");
        if (f) {
            handle_option_c(f);
            fclose(f);
        } else {
            perror("Error opening file for -c");
        }
    }

    if (octal_mode) {
        FILE *f = fopen(input_file, "r");
        if (f) {
            handle_option_o(f);
            fclose(f);
        } else {
            perror("Error opening file for -o");
        }
    }

    if (version_mode) {
        handle_option_v();
    }

    return 0;
}
