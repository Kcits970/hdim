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

	dump(&args);
	args_cleanup(&args);
	return 0;

bad_usage:
	fprintf(stderr, "Usage: ./hdim [-V] [-F|-F8|-F16 <sequence>] [-M|-S] <filename> [-b|-c|-C|-d|-o|-x] [-n <length>] [-s <skip>]\n");
	return 1;
}
