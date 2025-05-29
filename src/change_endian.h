/*
기능 안쓴다함
*/

#ifndef CHANGE_ENDIAN_H
#define CHANGE_ENDIAN_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t swap16(uint16_t);
uint32_t swap32(uint32_t);

void print_as_endian(FILE*, int, int);

#endif