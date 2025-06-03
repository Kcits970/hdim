#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "args.h"

FILE *fopen_shift(const char *fn, int s)
{
	FILE *f = fopen(fn, "rb");
	if (!f)
	{
		perror("fopen");
		return 0;
	}

	if (fseek(f, s, SEEK_SET))
	{
		perror("fseek");
		return 0;
	}

	return f;
}

int args_validate(struct args_struct *args)
{
	if (args->V)
		return 1;

	if (!args->fn1 || !(args->f1 = fopen_shift(args->fn1, args->s)))
		return 0;

	if (args->F + args->F8 + args->F16 > 1)
		return 0;

	if (args->fn2 && !(args->f2 = fopen_shift(args->fn2, args->s)))
		return 0;

	if (args->M != args->S)
		return 1;

	if (args->M && args->S)
		return 0;

	int output_cnt = args->b + args->c + args->C + args->d + args->o + args->x;
	if (output_cnt > 1)
		return 0;
	
	if (!output_cnt)
		args->x = 1;

	if (!args->n)
		args->n = INT_MAX;

	return 1;
}

static inline int __args_compare(char *arg, char *short_name, char *long_name)
{
	return !strcmp(arg, short_name) || !strcmp(arg, long_name);
}

static inline int __parse_int(char *arg, int lbound, int ubound, int *res)
{
	for (int i = 0; arg[i]; i++)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return 0;
	}

	int n = atoi(arg);
	if (n < lbound || n > ubound)
		return 0;

	*res = n;
	return 1;
}

int args_init(struct args_struct *args, int argc, char **argv)
{
	for(int i = 1; i < argc; i++)
	{
		// main arguments.
		if (__args_compare(argv[i], "-b", "--one-byte-octal"))
			args->b = 1;

		else if (__args_compare(argv[i], "-c", "--one-byte-char"))
			args->c = 1;

		else if (__args_compare(argv[i], "-C", "--canonical"))
			args->C = 1;

		else if (__args_compare(argv[i], "-d", "--two-bytes-decimal"))
			args->d = 1;

		else if (__args_compare(argv[i], "-n", "--length"))
		{
			if (++i >= argc || !__parse_int(argv[i], 0, INT_MAX, &args->n))
				return 0;
		}

		else if (__args_compare(argv[i], "-o", "--two-bytes-octal"))
			args->o = 1;

		else if (__args_compare(argv[i], "-s", "--skip"))
		{
			if (++i >= argc || !__parse_int(argv[i], 0, INT_MAX, &args->s))
				return 0;
		}

		else if (__args_compare(argv[i], "-x", "--two-bytes-hex"))
			args->x = 1;

		else if (__args_compare(argv[i], "-V", "--version"))
			args->V = 1;

		// extra arguments.
		else if (__args_compare(argv[i], "-D", "--difference"))
		{
			if (i+1 >= argc)
				return 0;

			args->fn2 = argv[++i];
		}

		else if (__args_compare(argv[i], "-M", "--md5"))
			args->M = 1;

		else if (__args_compare(argv[i], "-S", "--sha256"))
			args->S = 1;
		
		else if (__args_compare(argv[i], "-F", "--find"))
		{
			if (i+1 >= argc)
				return 0;

			args->F = 1;
			args->pat = argv[++i];
		}

		else if (__args_compare(argv[i], "-F8", "--find-octal"))
		{
			if (i+1 >= argc)
				return 0;

			args->F8 = 1;
			args->pat = argv[++i];
		}

		else if (__args_compare(argv[i], "-F16", "--find-hex"))
		{
			if (i+1 >= argc)
				return 0;

			args->F16 = 1;
			args->pat = argv[++i];
		}

		// if the argument matches nothing, then it must be a filename.
		else
			args->fn1 = argv[i];
	}

	return args_validate(args);
}

void args_cleanup(struct args_struct *args)
{
	if (args->f1)
		fclose(args->f1);

	if (args->f2)
		fclose(args->f2);
}
