#ifndef ARGS_H
#define ARGS_H

struct args_struct
{
	/*
	 * fn1: main filename.
	 * fn2: filename for difference comparison.
	 *
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
	 * F8: --find-octal
	 * F16: --find-hex
	 */

	char *fn1, *fn2, *pat;
	FILE *f1, *f2;
	int b, c, C, d, n, o, s, x, V, S, M, F, F8, F16;
};

int args_init(struct args_struct *, int, char **);
void args_cleanup(struct args_struct *);

#endif
