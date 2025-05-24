#include <stdio.h>
#include <errno.h>
#include "dump.h"
#include "options.h"

Op opt = {
	.rule = 1, // 처음엔 1, 잘못된 경우의 수 한 곳이라도 걸리면 -1로 변해서 오류 
	.num_of_files = 0, // opt_check에서 .txt로 끝나는 문자열을 읽을 때마다 1씩 증가
	.option = { "-n", "-C", "-b", "-D", "-d", "-o", "-c", "-v" }, // 존재하는 옵션
};

D Diff = {
	.diff_size = 0
};


int main(int argc, char **argv)
{
	
	set_opt(argc, argv); // 옵션 세팅
	
	if (opt.rule == -1) // set_opt에서 잘못된 입력으로 판단하면 rule에 -1값 저장
		goto bad_usage;

	if (opt.v == 1) {
        handle_option_v();
		return 0;
    }

	for(int i=0;i<opt.num_of_files;i++) // 파일의 개수만큼 반복
	{
	FILE *f = fopen(argv[opt.files[i]], "r"); //opt.files[i] -> 해당하는 argv의 인덱스
	if (!f)
	{
		fprintf(stderr, "./hdim: ");
		perror(argv[opt.files[i]]);
		return 1;
	}
	
	if(opt.c == 1)
		handle_option_c(f);
	else if(opt.o == 1)
		handle_option_o(f);
	else if(opt.d == 1)
		handle_option_d(f);
	else 
		dump(f);

	fclose(f);
	}
	return 0;

bad_usage:
	fprintf(stderr, "Usage: ./hdim <file>\n");
	return 1;
}
