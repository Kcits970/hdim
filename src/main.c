#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

		goto bad_usage;

	if (args.V)
	{
		puts("hdim (hexdump-improved) 1.0.0");
		return 0;
	}

	return 0;


bad_usage:
	fprintf(stderr, "Usage: ./hdim [-V] <filename> [-b|-c|-C|-d|-o|-x] [-n <length>] [-s <skip>]\n");
	return 1;

bad_usage_find:
	fprintf(stderr, "Usage: ./hdim <file> --find <keyword>\n");
	return 1;
}


