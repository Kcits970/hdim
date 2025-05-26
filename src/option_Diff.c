#include "dump.h"

void dump_D()
{
	diff_index();
    char ch[4];
    char fmt[5]; 
    char full_fmt[32];
    int chunk_count;
    if(opt.b == 1) // 8비트 출력, 1비트씩 출력하고 공백출력
    {
        strcpy(ch, "03o");
        chunk_count = 1;
    }
    else // 16진수 출력, 2비트씩 출력하고 공백출력
    {
        strcpy(ch, "02x");
        chunk_count = 2;
    }

    snprintf(fmt, sizeof fmt, "%%%s", ch);  
    snprintf(full_fmt, sizeof full_fmt, "\033[1;31m%s\033[0m", fmt); //빨간색 문자열 출력

	unsigned int row = 0;
	unsigned int off = 0;
	
	while (off < strlen(Diff.txt_buf[0]) && off < strlen(Diff.txt_buf[1])) 
	{
		fprintf(stdout, "%08x ", row);
		size_t i;
		for(int k=0;k<2;k++)
		{
			if(k==1)
				printf(" |"); // | 비트출력 구분선 

			int chunk = 0;
			
			for (i = 0; i < 0x10 && off + i < strlen(Diff.txt_buf[k]); i += 1)
			{
				int found = 0;
				
				for(int j = 0; j<Diff.diff_size;j++) // 두 파일에서 다른 곳의 인덱스 저장한 곳
				{                     
					if(off+i == Diff.diff[j]) // 해당하지 않으면 : 값다, 해당하면 : 다르다(빨간색으로 출력)
						found++;
				}

				if(chunk % chunk_count == 0)
					printf(" ");
				if(found == 1 && k == 1) // 차이, 두번째 파일 비트 출력때 빨간색으로 숫자 출력
				{
                    snprintf(full_fmt, sizeof full_fmt, "\033[1;31m%s\033[0m", fmt);
					printf(full_fmt, Diff.txt_buf[k][off+i]);
					chunk++;
				}
				else
				{
					printf(fmt, Diff.txt_buf[k][off+i]);
					chunk++;
				}
				if(opt.C == 1)
				{
					if(found == 1 && k == 1) //차이, 두번째 파일 글자 출력때 빨간색으로 출력
					{	
						char full_fmt1[32];
						snprintf(full_fmt1, sizeof full_fmt1, "\x1b[1;31m%c\x1b[0m", Diff.txt_buf[k][off+i]);
						strcpy(Diff.buf_C[k][i], full_fmt1);
					}
					else // 나머지 모든 경우는 흰색 글자로 그냥 저장 
					{
						char s[2];
						s[0] = Diff.txt_buf[k][off+i];
						s[1] = '\0';
						strcpy(Diff.buf_C[k][i], s);
					}
				}
			}
		if(opt.C == 1)
			Diff.buf_C[k][i][0] = '\0';

		if (i < 0x10) {           // 열정렬
    		int pad = 0x10 - i;             
   		for (int p = 0; p < pad; p++) {
			if(chunk % chunk_count == 0)
				printf(" ");
			if(opt.b == 1)
			{
				printf("   ");
				chunk++;
			}
			else{
				printf("  ");
				chunk++;
			}
    	}
		}
	}

	if(opt.C == 1)
	{
		for(int k=0;k<2;k++)
		{
			int z = 0;
			printf(" |");
			while ( Diff.buf_C[k][z][0] != '\0')
			{
				printf("%s", Diff.buf_C[k][z]); // 문자열 출력 = 문자 1개 출력
				z++;
			}
			if (z < 0x10) {                              
   				for (int p = 0; p < 0x10 -z; ++p) 
            		printf(" ");
			}
			printf("|"); 
		}
	}

	fprintf(stdout, "\n");
	row += 0x10, off += 0x10;
	}	
}

void diff_index()
{
	for(int i= 0; i<1024;i++)
	{
		if(Diff.txt_buf[0][i] != Diff.txt_buf[1][i])
		{
			Diff.diff[Diff.diff_size] = i;
			Diff.diff_size++;
		}
	}
}