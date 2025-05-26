#include "dump.h"
// argv에 저장된 문자열을 하나씩 검사해서 해당하는 옵션에 (int) 1 값을 저장해서 체크
// 잘못된 입력일 경우 opt.rule에 -1값을 넣어서 bad_usage
void set_opt(int argc, char **argv)
{
    int i;

	for(i=1;argv[i] != NULL;i++)
	{
		opt_check(argv[i], i);

		if (strcmp(argv[i], "-V") == 0 || strcmp((argv[i]), "-v") == 0)
			opt.v = 1;

		if (strcmp(argv[i], "-d") == 0) 
            opt.d = 1;

		if (strcmp(argv[i], "-c") == 0)
			opt.c = 1;
		
		if (strcmp(argv[i], "-D") == 0)
			opt.D = 1;

		if (strcmp(argv[i], "-o") == 0)
			opt.o = 1;

		if(strcmp(argv[i], "-C") == 0 )
			opt.C = 1;

		if(strcmp(argv[i], "-b") == 0 )
			opt.b = 1;

		if(strcmp(argv[i], "-n") == 0 ) // 앞이 n옵션인데 뒤가 숫자가 아닐 경우
   		{
        	if(atoi(argv[i+1]) == 0)
            	opt.rule = -1;
        else
            opt.n = atoi(argv[i+1]); 
   		}

		if(atoi(argv[i+1]) != 0) // 뒤가 숫자인데 앞에가 n옵션이 아닐 경우 
		{
			if(strcmp(argv[i], "-n") != 0)
				opt.rule = -1;
		}
	}

    if(opt.D == 1 && !(opt.num_of_files == 2)) // D옵션인데 파일이 2개가 아닌 경우
         opt.rule = -1;

     if(opt.D == 1 && (opt.num_of_files == 2))
		opt.D = 1;

	if(opt.v == 1 && argc != 2)
		opt.rule = -1;
	
	if((opt.num_of_files  < 1 || opt.num_of_files > 2) && opt.v != 1) // 파일의 수는 1개~2개, 범위를 넘어가면 오류, V옵션 제외
        opt.rule = -1;

    if(opt.num_of_files == 2 && opt.D == 0) // 파일이 2개인데 D옵션을 입력하지 않은 경우
        opt.rule = -1;
}


void opt_check(char *argv, int i) // 입력된 문자열이 존재하는 옵션, 자연수, txt파일인 지 검사(+txt 파일 인덱스 저장)
{
	int count = 0;
	for(int j=0;j<8;j++) // 존재하는 옵션일 경우
	{
		if(strcmp(argv, opt.option[j]) == 0)
			count++;
	}
	if(atoi(argv) != 0) // 정수일 경우 (자연수)
		count++;
	
	char *s;

	s = argv;
	for(int j=0;j<strlen(s);j++) // txt파일일 경우
	{
		if(s[j] == '.')
		{
			s = &s[j+1];
			if(strcmp(s, "txt") == 0)
			{
				switch(opt.num_of_files)
				{
					case 0:
						opt.files[0] = i; // main.c 에서 opt.files에 저장된 인덱스로 파일 열기
						break;
					case 1:
						opt.files[1] = i;
						break;
				}
					opt.num_of_files++;
					count++;
			}
		}
	}

	if(count == 0) // 위에 한 곳이라도 해당하지 않으면 오류
		opt.rule = -1;
}

