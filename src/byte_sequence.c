#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "args.h"
#include "util.h"

static inline int isodigit(int c)
{
	return c >= '0' && c <= '8';
}

static inline int ctoi(int c)
{
	if (c >= '0' && c <= '9')
		return c-'0';

	if (c >= 'A' && c <= 'F')
		return c-'A'+10;

	if (c >= 'a' && c <= 'f')
		return c-'a'+10;

	return -1;
}

int parse_bytes(const char *str, char *res, int base)
{
	const char *s = str;
	int len = 0;

	while (s[0])
	{
		if (isspace(s[0]))
		{
			s++;
			continue;
		}

		if (base == 16)
		{
			if (!isxdigit(s[0]) || !isxdigit(s[1]))
				return -1;

			res[len++] = ctoi(s[0]) << 4 | ctoi(s[1]);
			s += 2;
		}

		// base 8 (octal).
		else
		{
			if (!isodigit(s[0]) || !isodigit(s[1]) || !isodigit(s[2]))
				return -1;

			res[len++] = ctoi(s[0]) << 6 | ctoi(s[1]) << 3 | ctoi(s[2]);
			s += 3;
		}
	}

	return len;
}

// kmp pre-computation.
void build_lps(const char *str, int str_len, int *lps)
{
	// str_len: length of 'str'.
	// len: length of currently tracking prefix/suffix.
	
	int len = 0;
	lps[0] = 0;

	for (int i = 1; i < str_len;)
	{
		if (str[i] == str[len])
			lps[i++] = ++len;

		else if (len)
			len = lps[len-1];

		else
			lps[i++] = 0;
	}
}

void find_byte_sequence(FILE *f, struct args_struct *args)
{
	static char buf[1024];

	int len = strlen(args->pat);
	char *pat = malloc(len+1);
	int *lps = malloc(len * sizeof(int));

	if (args->F)
		strcpy(pat, args->pat);

	else
		len = parse_bytes(args->pat, pat, args->F8 ? 8 : 16);

	if (len <= 0)
		goto cleanup;

	build_lps(pat, len, lps);

	int read_sz, off = args->s, j = 0;
	while (read_sz = fread(buf, 1, imin2(args->n-off, 1024), f))
	{
		for (int i = 0; i < read_sz; i++, off++)
		{
			while (j && buf[i] != pat[j])
				j = lps[j - 1];

			if (buf[i] == pat[j])
				j++;

			if (j == len)
			{
				printf("Found matching pattern at 0x%08x\n", off-len+1);
				j = lps[j - 1];
			}
		}
	}

cleanup:
	free(pat);
	free(lps);
}
