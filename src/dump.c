#include <stdio.h>
#include "args.h"
#include "dump.h"
#include "util.h"
#include "checksum.h"
#include "find.h"

static inline char __to_printable(char ch)
{
	if (ch >= ' ' && ch <= '~')
		return ch;

	return '.';
}

static char space[8192];

int dump_buf(FILE *f, struct args_struct *args, char *out, int from)
{
	static char buf[1024];
	static char tmp[16];

	int read_sz = fread(buf, sizeof(char), imin2(args->n-from, 1024), f);
	int off = 0, write_sz = 0;

	if (read_sz & 1)
		buf[read_sz] = 0;

	while (off < read_sz)
	{
		int len = imin2(read_sz-off, 16);
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

			else if (args->C)
			{
				sprintf(tmp, "%02x", (unsigned int) buf[off+i]);
				sprintf(out+write_sz, "%.2s", i < len ? tmp : space);
				i++, write_sz+=2;
			}

			else if (args->x)
			{
				sprintf(tmp, "%04x", (unsigned int) buf[off+i+1] << 8 | buf[off+i]);
				sprintf(out+write_sz, "%.4s", i < len ? tmp : space);
				i+=2, write_sz+=4;
			}

			else if (args->d)
			{
				sprintf(tmp, "%05u", (unsigned int) buf[off+i+1] << 8 | buf[off+i]);
				sprintf(out+write_sz, "%.5s", i < len ? tmp : space);
				i+=2, write_sz+=5;
			}

			else if (args->o)
			{
				sprintf(tmp, "%06o", (unsigned int) buf[off+i+1] << 8 | buf[off+i]);
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

int dump_line(struct args_struct *args, const char *buf, int from)
{	
	for (int i = 0; i < 16;)
	{
		if (args->b)
		{
			printf("%.3s ", buf+from);
			i++, from+=3;
		}

		else if (args->c)
		{
			putchar(buf[from]);
			putchar(' ');
			i++, from++;
		}

		else if (args->C)
		{
			printf("%.2s ", buf+from);
			i++, from+=2;
		}

		else if (args->x)
		{
			printf("%.4s ", buf+from);
			i+=2, from+=4;
		}

		else if (args->d)
		{
			printf("%.5s ", buf+from);
			i+=2, from+=5;
		}

		else if (args->o)
		{
			printf("%.6s ", buf+from);
			i+=2, from+=6;
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
			putchar(buf[from++]);

		putchar(' ');
	}

	return from;
}

int dump_part(FILE *f, struct args_struct *args, int from)
{
	static char buf[8192];

	int read_sz = dump_buf(f, args, buf, from);
	int off = 0, write_sz = 0;
	int row = from/16*16;

	while (off < read_sz)
	{
		printf("%08x ", row + args->s);
		write_sz = dump_line(args, buf, write_sz);
		putchar('\n');

		off += 16;
		row += 16;
	}

	return read_sz;
}

#define RED "\033[31m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

int dump_line_diff(struct args_struct *args, const char *buf, const char *cmp, int from)
{
	for (int i = 0; i < 16;)
	{
		if (args->b)
		{
			if (memcmp(buf+from, cmp+from, 3))
				printf(BOLD RED);

			printf("%.3s ", buf+from);
			i++, from+=3;
		}

		else if (args->c)
		{
			if (buf[from] != cmp[from])
				printf(BOLD RED);

			putchar(buf[from]);
			putchar(' ');
			i++, from++;
		}

		else if (args->C)
		{
			if (memcmp(buf+from, cmp+from, 2))
				printf(BOLD RED);

			printf("%.2s ", buf+from);
			i++, from+=2;
		}

		else if (args->x)
		{
			if (memcmp(buf+from, cmp+from, 4))
				printf(BOLD RED);

			printf("%.4s ", buf+from);
			i+=2, from+=4;
		}

		else if (args->d)
		{
			if (memcmp(buf+from, cmp+from, 5))
				printf(BOLD RED);
			
			printf("%.5s ", buf+from);
			i+=2, from+=5;
		}

		else if (args->o)
		{
			if (memcmp(buf+from, cmp+from, 6))
				printf(BOLD RED);

			printf("%.6s ", buf+from);
			i+=2, from+=6;
		}

		else
		{
			// execution will never reach this point.
		}

		printf(RESET);
	}

	if (args->C)
	{
		printf("| ");

		for (int i = 0; i < 16; i++)
		{
			if (buf[from] != cmp[from])
				printf(BOLD RED);

			putchar(buf[from++]);
			printf(RESET);
		}

		putchar(' ');
	}

	return from;
}

int dump_diff(FILE *f1, FILE *f2, struct args_struct *args, int from)
{
	static char buf1[8192];
	static char buf2[8192];

	int read_sz1 = dump_buf(f1, args, buf1, from);
	int read_sz2 = dump_buf(f2, args, buf2, from);
	int read_sz = imin2(read_sz1, read_sz2);
	int read_sz_max = imax2(read_sz1, read_sz2);
	int off = 0, write_sz = 0, write_sz2 = 0;
	int row = from/16*16;

	while (off < read_sz)
	{
		printf("%08x ", row + args->s);
		dump_line_diff(args, buf1, buf2, write_sz);
		printf("| ");
		write_sz = dump_line_diff(args, buf2, buf1, write_sz);
		putchar('\n');

		off += 16;
		row += 16;
	}

	while (off < read_sz_max)
	{
		printf("%08x ", row + args->s);

		if (read_sz1 == read_sz_max)
		{
			write_sz = dump_line_diff(args, buf1, space, write_sz);
			printf(RESET);
			printf("| ");
			dump_line(args, space, 0);
		}
		else
		{
			dump_line(args, space, 0);
			printf("| ");
			printf(BOLD RED);
			write_sz = dump_line_diff(args, buf2, space, write_sz);
			printf(RESET);
		}

		putchar('\n');
		off += 16;
		row += 16;
	}

	return read_sz_max;
}

void dump(struct args_struct *args)
{
	if (args->V)
	{
		puts("hdim (hexdump-improved) 1.0.0");
		return;
	}

	FILE *f1 = args->f1;
	FILE *f2 = args->f2;
	memset(space, ' ', sizeof(space));

	if (args->F || args->F8 || args->F16)
	{
		find_byte_sequence(f1, args);
		return;
	}

	if (args->M)
	{
		md5(f1);
		return;
	}

	if (args->S)
	{
		sha256(f1);
		return;
	}

	if (f2)
	{
		int from = 0, sz;
		while (sz = dump_diff(f1, f2, args, from))
			from += sz;
	}
	else
	{
		int from = 0, sz;
		while (sz = dump_part(f1, args, from))
			from += sz;
	}
}
