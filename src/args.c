#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "args.h"

int args_set_f(struct args_struct *args, char *str)
{
	if (args->f)
		return 0;

	args->f = fopen(str, "r");
	if (!args->f)
	{
		perror("fopen");
		return 0;
	}

	return 1;
}

int args_set_D(struct args_struct *args, char *str)
{
	if (args->D)
		return 0;

	args->D = fopen(str, "r");
	if (!args->D)
	{
		perror("fopen");
		return 0;
	}

	return 1;
}

int args_set_n(struct args_struct *args, char *str)
{
	if (args->n)
		return 0;

	char *ptr;
	long int n = strtol(str, &ptr, 10);
	if (*ptr || n <= 0 || n > INT_MAX)
		return 0;

	args->n = n;
	return 1;
}

int args_validate(struct args_struct *args)
{
	if (!args->V && !args->f)
		return 0;

	int output_cnt = args->b + args->c + args->C + args->d + args->x + args->o;
	if (output_cnt > 1)
		return 0;
	
	if (!output_cnt)
		args->x = 1;

	if (args->n && args->s)
		return 0;

	return 1;
}

int args_init(struct args_struct *args, int argc, char **argv)
{
	for(int i = 1; i < argc; i++)
	{
		if (argv[i][0] != '-')
		{
			if (!args_set_f(args, argv[i]))
				return 0;

			continue;
		}

		if (strlen(argv[i]) != 2)
			return 0;


		switch (argv[i][1])
		{
			case 'V':
				args->V = 1; break;

			case 'd':
				args->d = 1; break;

			case 'c':
				args->c = 1; break;

			case 'D':
				if (i+1 > argc || !args_set_D(args, argv[i+1]))
					return 0;
				break;

			case 'o':
				args->o = 1; break;

			case 'C':
				args->C = 1; break;

			case 'b':
				args->b = 1; break;

			case 'x':
				args->x = 1; break;

			case 'n':
				if (i+1 > argc || !args_set_n(args, argv[i+1]))
					return 0;
				break;

			default:
				return 0;
		}
	}

	return args_validate(args);
}
