#ifndef DUMP_H
#define DUMP_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

typedef struct { // 보통 사용하는 Opt 구조체
    int n, C, b, D, d, c, v, o;
    int num_of_files;
    int files[2];
    int rule;
    char *option[8];
    unsigned char buf_C[1024];
} Op;

typedef struct { // D옵션에 사용할 구조체
    unsigned char txt_buf[2][1024]; // 두 파일의 buf 따로 저장 해놓고, option_D에서 사용
    int diff[1000]; // 문자가 다른 곳의 인덱스 따로 저장
    int diff_size; // 다른 문자 개수 
    char buf_C[2][1024][32]; // D, C 옵션 둘다 사용할 때 빨간색 글자를 저장하기 위해 문자열로 저장
} D;

extern Op opt; 
extern D Diff;

void dump(FILE*);
void dump_b(int count, char buf[]); 
void pad_column(int i);

void dump_D(); // D 옵션 함수들
void diff_index();

void set_opt(int argc, char **argv); // opt 세팅함수들 
void opt_check(char *argv, int i);

#endif
