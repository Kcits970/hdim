#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "args.h"

int args_validate(struct args_struct *args)
{
	if (args->V)
		return 1;

	if (!args->f1)
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

			args->f2 = argv[++i];
		}


		// extra: sha256, md5, find
        else if (__args_compare(argv[i], "-S", "--sha256")) {
			args->sha = 1;
		}
        else if (__args_compare(argv[i], "-M", "--md5"))    {
			args->md  = 1;
		}
		//ex) -F hello test.txt
        else if (__args_compare(argv[i], "-F", "--find")) {
            if (++i >= argc) return 0;
            args->find = 1;
            args->pattern = argv[i];
        }
		/*
		else if (__args_compare(argv[i], "-FH", "--findHex")) {
            if (++i >= argc) return 0;
            args->findHex = 1;
            args->pattern = argv[i];
        }*/

		// if the argument matches nothing, then it must be a filename.
		else
			args->f1 = argv[i];
	}

	return args_validate(args);
}
