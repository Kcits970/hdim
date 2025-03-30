#define BUF_SZ 1024

#include <stdio.h>
#include "dump.h"

unsigned char buf[BUF_SZ];

void dump(FILE *f)
{
	size_t count;
	unsigned int row = 0;

	while ((count = fread(buf, sizeof(unsigned char), BUF_SZ, f)) > 0)
	{
		if (count & 1)
			buf[count++] = 0;

		unsigned int off = 0;
		while (off < count)
		{
			fprintf(stdout, "%08x ", row);
			for (size_t i = 0; i < 0x10 && off + i < count; i += 2)
				printf("%02x%02x ", buf[off + i], buf[off + i+1]);
			fprintf(stdout, "\n");
			row += 0x10, off += 0x10;
		}
	}
}
