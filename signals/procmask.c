#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	sigset_t sigset;
	int index;

	sigemptyset(&sigset);
	sigaddset(&sigset,SIGINT);
	sigprocmask(SIG_BLOCK, &sigset, NULL);

	// 중요한 일 시작
	for (index =10; 0<index; index--)
	{
		printf("Count Down : %d\n",index);
		sleep(1);
	}

	printf("CTRL-C에 대한 블록을 해제합니다.\n");
	sigprocmask(SIG_UNBLOCK,&sigset,NULL);
	printf("카운트 다운 중 CTRL-C키를 눌렀다면, 이 문장은 출력되지 않음, procmask 함수가 실행 된 후 SIGINT 신호 받았던 것을 바로 신호 수신하여 종료됨.\n");

	while(1)
	{
		printf("test\n");
		sleep(4);
	}
	return 0;
}

