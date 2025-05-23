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

    // 인자 파싱 (옵션 순서 무관, -- 지원)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--d") == 0) {
            dump_mode = 1;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--c") == 0) {
            char_mode = 1;
        } else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--V") == 0) {
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
        fprintf(stderr, "Usage: ./hdim [-d] [-c] [-V] [-o] <file>\n");
        return 1;
    }

    // 파일 열기
    FILE *f = fopen(input_file, "r");
    if (!f) {
        fprintf(stderr, "./hdim: ");
        perror(input_file);
        return 1;
    }

    // 옵션별 기능 실행
    if (dump_mode)
        handle_option_d(f);       // 2바이트 10진수 출력
    if (char_mode)
        handle_option_c(f);       // 문자 그대로 출력
    if (version_mode)
        handle_option_v();        // 버전 정보 출력
    if (octal_mode)
        handle_option_o(f);       // 2바이트 8진수 출력

    fclose(f);
    return 0;
}
