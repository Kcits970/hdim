#include <stdio.h>
#include <errno.h>
#include "dump.h"

int main(int argc, char **argv)
{
	if (argc != 2)
		goto bad_usage;

	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "./hdim: ");
		perror(argv[1]);
		return 1;
	}

	dump(f);

	fclose(f);
	return 0;

bad_usage:
	fprintf(stderr, "Usage: ./hdim <file>\n");
	return 1;
}
