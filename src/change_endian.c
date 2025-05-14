#include <stdio.h.>
#include <stdlib.h>
#include "change_endian.h"

// 빅엔디안과 리틀엔디안 간의 16비트 데이터 교환 함수
unsigned short swap16(unsigned short value) {
    // 데이터를 8비트 단위로 나누고, 마지막과 처음의 위치를 바꿔서 재조합
    return (value >> 8) | (value << 8);
}

// 빅엔디안과 리틀엔디안 간의 32비트 데이터 교환 함수
unsigned int swap32(unsigned int value) {
    // 데이터를 8비트 단위로 나누고, 위치를 바꿔서 재조합
    return ((value >> 24) & 0xFF) | ((value >> 8) & 0xFF00) | ((value << 8) & 0xFF0000) | ((value << 24) & 0xFF000000);
}