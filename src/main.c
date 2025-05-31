#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "dump.h"
#include "args.h"

int main(int argc, char **argv)
{
	struct args_struct args;
	memset(&args, 0, sizeof(args));

	if (!args_init(&args, argc, argv))
		goto bad_usage;

	if (args.V)
	{
		puts("hdim (hexdump-improved) 1.0.0");
		return 0;
	}

	dump(&args);
	return 0;

bad_usage:
	fprintf(stderr, "Usage: ./hdim [-V] <filename> [-b|-c|-C|-d|-o|-x] [-M|-S] [-n <length>] [-s <skip>] [-F <byte-sequence>]\n");
	return 1;
}
