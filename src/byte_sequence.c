#define BUF_SZ 1024

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include "byte_sequence.h"

unsigned char buf[BUF_SZ];

// 헐사 문자열을 바이트 배열로 변환 (예: "ab cd" → {0xab, 0xcd})
size_t parse_keyword(const char *keyword, unsigned char *pattern) {
    size_t len = 0;
    while (*keyword && len < BUF_SZ) {
        while (*keyword && isspace(*keyword)) keyword++;
        if (!*keyword) break;
        unsigned int byte;
        if (sscanf(keyword, "%2x", &byte) != 1) break;
        pattern[len++] = (unsigned char)byte;
        keyword += 2;
    }
    return len;
}

// KMP 실패 함수 테이블 생성
void compute_kmp_table(unsigned char *pattern, size_t m, int *lps) {
    int len = 0;
    lps[0] = 0;
    for (size_t i = 1; i < m;) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else {
            if (len != 0) len = lps[len - 1];
            else lps[i++] = 0;
        }
    }
}

// KMP 탐색 구현
void find_byte_sequence(FILE *f, char *keyword) {
    unsigned char pattern[BUF_SZ];
    size_t pattern_len = parse_keyword(keyword, pattern);
    if (pattern_len == 0) {
        fprintf(stderr, "Invalid pattern input.\n");
        return;
    }

    int *lps = malloc(sizeof(int) * pattern_len);
    if (!lps) {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }
    compute_kmp_table(pattern, pattern_len, lps);

    size_t count, total_read = 0;
    int j = 0; // pattern index

    while ((count = fread(buf, 1, BUF_SZ, f)) > 0) {
        for (size_t i = 0; i < count; i++) {
            while (j > 0 && buf[i] != pattern[j])
                j = lps[j - 1];

            if (buf[i] == pattern[j]) j++;

            if (j == pattern_len) {
                size_t match_offset = total_read + i - pattern_len + 1;
                size_t row = match_offset / 16;
                size_t col = match_offset % 16;
                printf("[+] Pattern found at offset 0x%08zx (row: %zu, col: %zu)\n", match_offset, row, col);
                j = lps[j - 1];
            }
        }
        total_read += count;
    }

    free(lps);
    rewind(f); // rewind for later use
}
