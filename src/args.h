#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>

struct args_struct
{
	FILE *f, *D;
	int V, d, c, o, C, b, x, n, s;
};

int args_init(struct args_struct*, int, char**);

#endif
