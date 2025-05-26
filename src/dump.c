#define BUF_SZ 1024

#include <stdio.h>
#include "dump.h"
#include "options.h"

unsigned char buf[BUF_SZ];

int read_f = 0;

void dump(FILE *f)
{
	size_t count;
	unsigned int row = 0;


	while ((count = fread(buf, sizeof(unsigned char), BUF_SZ, f)) > 0 )
	{

		if (opt.n > 0)       // n 옵션
			count = opt.n; 

		if (count & 1)
			buf[count++] = 0;
		
		unsigned int off = 0;

		if(opt.D == 1)
		{
			if(read_f < 2) 
			{
				memcpy(Diff.txt_buf[read_f], buf, count);
				Diff.txt_buf[read_f][count] = '\0';
				read_f++;

				if(read_f < 2) // D옵션일 경우, 첫번째 파일은 읽고 return 
					return;
			}
		}
			
		if(read_f == 2) // 두번째 파일이 들어오면 dump_D
		{
			dump_D();
			return;
		}

		if(opt.b == 1) // b 옵션
			dump_b(count, buf);
		else
		{
		while (off < count)
		{
			size_t i;
			fprintf(stdout, "%08x ", row);
			for (i = 0; i < 0x10 && off + i < count; i += 2)
			{
				printf("%02x%02x ", buf[off + i], buf[off + i+1]);
				if(opt.C == 1) // 16비트 출력마다 해당 줄의 문자열을 출력하기 위해 buf_C에 따로 저장
				{
					opt.buf_C[i] = buf[off + i]; 
					opt.buf_C[i + 1] = buf[off + i + 1];
				}
			}
			if(opt.C == 1)
			{
				opt.buf_C[i] = '\0';
				pad_column(i); // 한 줄에 16비트보다 적을 때 열 정렬 맞추기
				printf(" |%s|", opt.buf_C);
			}
			fprintf(stdout, "\n");
			row += 0x10, off += 0x10;
		}
		}
	
	}
}

void dump_b(int count, char buf[])
{
	unsigned int row = 0;
	unsigned int off = 0;

	while(off < count)
	{
		size_t i;
		fprintf(stdout, "%08x ", row);  
		for (i = 0; i < 0x10 && off + i < count; i += 1)
		{
			printf("%03o ", buf[off+i]);
			if(opt.C == 1)
				opt.buf_C[i] = buf[off + i];
		}
		if(opt.C == 1)
		{
			pad_column(i); // 16비트보다 적을 때 열 정렬 맞추고 문자열 출력
			opt.buf_C[i] = '\0';
			printf(" |%s|", opt.buf_C);
		}
		fprintf(stdout, "\n");
		row += 0x10, off += 0x10; 
		}
}

void pad_column(int i)
{
	if (i < 0x10) {                      
    	int pad = 0x10 - i;             
		int g = opt.b ? 1 : 2;
   		for (int p = 0; p < pad; p+=g) {
           	printf(opt.b ? "    " : "     ");
    	}
	}
}