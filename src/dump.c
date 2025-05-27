#include <stdio.h>
#include "args.h"
#include "dump.h"

static inline char __to_printable(char ch)
{
	if (ch >= ' ' && ch <= '~')
		return ch;

	return '.';
}

int __dump(struct args_struct *args, char *out)
{
	static char buf[1024];
	static char tmp[16];
	static const char *space = "                ";

	FILE *f = args->f;
	int read_sz = fread(buf, sizeof(char), 1024, f);
	int off = 0, write_sz = 0;

	if (read_sz & 1)
		buf[read_sz] = 0;

	while (off < read_sz)
	{
		int len = read_sz-off >= 16 ? 16 : read_sz-off;
		for (int i = 0; i < 16;)
		{
			if (args->b)
			{
				sprintf(tmp, "%03o", (unsigned int) buf[off+i]);
				sprintf(out+write_sz, "%.3s", i < len ? tmp : space);
				i++, write_sz+=3;
			}

			else if (args->c)
			{
				out[write_sz++] = i < len ? __to_printable(buf[off+i]) : ' ';
				i++;
			}

			else if (args->C || args->x)
			{
				sprintf(tmp, "%04x", (unsigned int) buf[off+i] << 8 | buf[off+i+1]);
				sprintf(out+write_sz, "%.4s", i < len ? tmp : space);
				i+=2, write_sz+=4;
			}

			else if (args->d)
			{
				sprintf(tmp, "%05u", (unsigned int) buf[off+i] << 8 | buf[off+i+1]);
				sprintf(out+write_sz, "%.5s", i < len ? tmp : space);
				i+=2, write_sz+=5;
			}

			else if (args->o)
			{
				sprintf(tmp, "%06o", (unsigned int) buf[off+i] << 8 | buf[off+i+1]);
				sprintf(out+write_sz, "%.6s", i < len ? tmp : space);
				i+=2, write_sz+=6;
			}
			
			else
			{
				// something is wrong if execution happens here...
			}
		}

		if (args->C)
		{
			for (int i = 0; i < 16; i++)
				out[write_sz++] = i < len ? __to_printable(buf[off+i]) : ' ';
		}

		off += len;
	}

	return read_sz;
}

int dump(struct args_struct *args)
{
	static char buf[8192];

	int read_sz = __dump(args, buf);
	int off = 0, write_sz = 0, row = 0;

	while (off < read_sz)
	{
		for (int i = 0; i < 16;)
		{
			if (args->b)
			{
				printf("%.3s ", buf+write_sz);
				i++, write_sz+=3;
			}

			else if (args->c)
			{
				putchar(*(buf+write_sz));
				i++, write_sz++;
			}

			else if (args->C || args->x)
			{
				printf("%.4s ", buf+write_sz);
				i+=2, write_sz+=4;
			}

			else if (args->d)
			{
				printf("%.5s ", buf+write_sz);
				i+=2, write_sz+=5;
			}

			else if (args->o)
			{
				printf("%.6s ", buf+write_sz);
				i+=2, write_sz+=6;
			}

			else
			{
				// execution will never reach this point.
			}
		}

		if (args->C)
		{
			printf("| ");
			for (int i = 0; i < 16; i++)
				putchar(*(buf+write_sz++));
		}

		putchar('\n');
		off += 16;
	}

	return read_sz;
}
