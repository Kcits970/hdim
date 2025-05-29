#include <stdio.h>
#include "args.h"
#include "dump.h"
#include "util.h"
#include "checksum.h"
#include "byte_sequence.h"

static inline char __to_printable(char ch)
{
	if (ch >= ' ' && ch <= '~')
		return ch;

	return '.';
}

int dump_buf(FILE *f, struct args_struct *args, char *out)
{
	static char buf[1024];
	static char tmp[16];
	static const char *space = "                ";
	static int acc; // accumulation count.

	int read_sz = fread(buf, sizeof(char), imin2(args->n-acc, 1024), f);
	int off = 0, write_sz = 0;
	acc += read_sz;

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

			else if (args->C || args->x)
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

int dump_part(FILE *f, struct args_struct *args)
{
	static char buf[8192];
	static int row;

	int read_sz = dump_buf(f, args, buf);
	int off = 0, write_sz = 0;

	while (off < read_sz)
	{
		printf("%08x ", row);

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
				putchar(' ');
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
		row += 16;
	}

	return read_sz;
}

void dump(struct args_struct *args)
{
	FILE *f = fopen(args->f1, "rb");
	if (!f)
	{
		perror("fopen");
		return;
	}

	if (fseek(f, args->s, SEEK_SET))
	{
		perror("fseek");
		return;
	}

	while (dump_part(f, args));

	//sha 256 출력
	//./hdim -S ->파일의 sha256값 출력
    if (args->sha) {
        rewind(f);
        putchar('\n');
        sha256(f);     // void sha256(FILE *f) — 파일 전체 해시
        putchar('\n');
    }
	//md5 출력
	//./hdim -M ->파일의 md5값 출력
    if (args->md) {
        rewind(f);
        putchar('\n');
        md5(f);        // void md5(FILE *f)
        putchar('\n');
    }
    //바이트 시퀀스 탐색
    if (args->find) {
        rewind(f);
        putchar('\n');
        find_byte_sequence(f, args->pattern);  // offset을 출력만
        putchar('\n');
    }
	//파일 hex값 중 바이트 시퀀스 탐색 -> 쓸모 없을 듯
	/*
    if (args->findHex) {
        rewind(f);
        find_hex_sequence(f, args->pattern);  // offset을 출력만
        putchar('\n');
    }*/
}
