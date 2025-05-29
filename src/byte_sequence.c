// byte_sequence.h
#ifndef BYTE_SEQUENCE_H
#define BYTE_SEQUENCE_H

#include <stdio.h>

// -F: raw byte sequence search (KMP)
void find_byte_sequence(FILE *f, const char *keyword);
// -FH: search in hex representation of file contents
void find_hex_sequence(FILE *f, const char *keyword);

#endif // BYTE_SEQUENCE_H


// byte_sequence.c
#include "byte_sequence.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define BUF_SZ 1024
static unsigned char buf[BUF_SZ];

// 헥사 문자열을 바이트 배열로 변환 (예: "ab cd" → {0xab, 0xcd})
static size_t parse_keyword(const char *keyword, unsigned char *pattern) {
    const char *p = keyword;
    size_t len = 0;
    // 헥사 모드
    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        if (!isxdigit((unsigned char)p[0]) || !isxdigit((unsigned char)p[1])) { len = 0; break; }
        unsigned int byte;
        sscanf(p, "%2x", &byte);
        pattern[len++] = (unsigned char)byte;
        p += 2;
    }
    if (len > 0) return len;
    // ASCII 모드
    len = strlen(keyword);
    for (size_t i = 0; i < len; i++) pattern[i] = (unsigned char)keyword[i];
    return len;
}

// KMP 실패함수
static void compute_kmp_table(const unsigned char *pattern, size_t m, int *lps) {
    int length = 0;
    lps[0] = 0;
    for (size_t i = 1; i < m; ) {
        if (pattern[i] == pattern[length])
            lps[i++] = ++length;
        else if (length)
            length = lps[length - 1];
        else
            lps[i++] = 0;
    }
}

// -F: raw bytes
void find_byte_sequence(FILE *f, const char *keyword) {
    unsigned char pattern[BUF_SZ];
    size_t plen = parse_keyword(keyword, pattern);
    if (plen == 0) { fprintf(stderr, "Invalid pattern input.\n"); return; }
    int *lps = malloc(sizeof(int) * plen);
    if (!lps) { fprintf(stderr, "Memory allocation failed.\n"); return; }
    compute_kmp_table(pattern, plen, lps);

    size_t nread = 0, total = 0;
    int j = 0;
    while ((nread = fread(buf, 1, BUF_SZ, f)) > 0) {
        for (size_t i = 0; i < nread; ++i) {
            while (j > 0 && buf[i] != pattern[j]) j = lps[j - 1];
            if (buf[i] == pattern[j]) j++;
            if (j == (int)plen) {
                size_t off = total + i - plen + 1;
                printf("[+] Pattern found at offset 0x%08zx\n", off);
                j = lps[j - 1];
            }
        }
        total += nread;
    }
    free(lps);
    rewind(f);
}

// -FH: search in hex string
void find_hex_sequence(FILE *f, const char *keyword) {
    // 준비: 키워드 소문자화
    size_t klen = strlen(keyword);
    char *key = malloc(klen + 1);
    for (size_t i = 0; i < klen; ++i) key[i] = tolower((unsigned char)keyword[i]);
    key[klen] = '\0';

    // 파일 전체 읽기
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *data = malloc(fsize);
    if (!data) { perror("malloc"); free(key); return; }
    fread(data, 1, fsize, f);

    // 헥사 문자열 생성
    char *hex = malloc(fsize * 2 + 1);
    for (long i = 0; i < fsize; ++i)
        sprintf(hex + i*2, "%02x", data[i]);
    hex[fsize*2] = '\0';

    // 검색
    char *p = hex;
    while ((p = strstr(p, key)) != NULL) {
        size_t pos = p - hex;
        size_t off = pos / 2;
        size_t row = off / 16;
        size_t col = off % 16;
        printf("[+] HEX pattern '%s' at offset 0x%08zx (row:%zu, col:%zu)\n",
               keyword, off, row, col);
        p += 1;
    }

    // 정리
    free(hex);
    free(data);
    free(key);
    rewind(f);
}
