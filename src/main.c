#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "byte_sequence.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		goto bad_usage;

	FILE *f = fopen(argv[1], "r");
	if (!f)
	{
		fprintf(stderr, "./hdim: ");
		perror(argv[1]);
		return 1;
	}

	for (int i = 2; i < argc; ++i)
    {
        if (strncmp(argv[i], "--", 2) == 0)
        {
            if (strcmp(argv[i] + 2, "find") == 0)
            {
                if (i + 1 < argc)
                {
                    find_byte_sequence(f, argv[i + 1]);
                }
                else
                {
                    goto bad_usage_find;
                }
            }
        }
    }

	fclose(f);
	return 0;


bad_usage:
	fprintf(stderr, "Usage: ./hdim <file>\n");
	return 1;

bad_usage_find:
	fprintf(stderr, "Usage: ./hdim <file> --find <keyword>\n");
	return 1;
}


