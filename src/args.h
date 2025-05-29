#ifndef ARGS_H
#define ARGS_H

struct args_struct
{
	char *f1, *f2;
	int b, c, C, d, n, o, s, x, V, sha,md,find;
//	int findHex; //sha 256(sha), md5(md), byte sequence(find) byte hex sequence(findHex)
	char *pattern; //find 할때 pattern 기록
};

int args_init(struct args_struct*, int, char**);

#endif
