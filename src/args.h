#ifndef ARGS_H
#define ARGS_H

struct args_struct
{
	/*
	 * b: --one-byte-octal
	 * c: --one-byte-char
	 * C: --canonical
	 * d: --two-bytes-decimal
	 * n: --length
	 * o: --two-bytes-octal
	 * s: --skip
	 * x: --two-bytes-hex
	 * V: --version
	 * M: --md5
	 * S: --sha256
	 * F: --find
	 */

	char *f1, *f2;
	int b, c, C, d, n, o, s, x, V, S, M, F;
	char *pattern;
};

int args_init(struct args_struct*, int, char**);

#endif
