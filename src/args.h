#ifndef ARGS_H
#define ARGS_H

struct args_struct
{
	char *f1, *f2;
	int b, c, C, d, n, o, s, x, V, sha, md, F;
	char *pattern;
};

int args_init(struct args_struct*, int, char**);

#endif
