#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>

pthread_t t_id;

int hp = 100;
// 무기가 0은 맨주먹이고 , 1은 몽둥이, 2는 총이다.
int weaponFlag = 0;

void *t_func(void *arg);
void h_attckWithGun(int signo);
void h_attackWithKnife(int signo);
void SIGINT_handler(int signo, siginfo_t* si);
void SIGUSR1_handler(int signo);
void SIGUSR2_handler(int signo);

struct sigaction sa_old; /* Old signal actions */
struct sigaction sa_new; /* New signal actions */

int main(void)
{
	sa_new.sa_handler = SIGINT_handler; /* Point to our function */
	sigemptyset(&sa_new.sa_mask); /* Clear mask */
	sa_new.sa_flags = SA_SIGINFO;; /* No special flags */

	sigaction(SIGINT,&sa_new,&sa_old);
	signal(SIGUSR1,&SIGUSR1_handler);
	signal(SIGUSR2,&SIGUSR2_handler);

	printf("=======================================================\n");
	printf("시그널을 활용한 상대 보스를 죽이는 게임입니다\n");
	printf("기본 무기는 맨주먹입니다\n");
	printf("무기를 바꾸려면 SIGUSR1 신호를 보내세요\n");
	printf("공격을 하려면 SIGUSR2 신호를 보내세요\n");
	printf("무기별 공격력 정보\n");
	printf("맨주먹 공격력 : 5\n");
	printf("몽둥이 공격력 : 10\n");
	printf("총 공격력 : 20\n");
	printf("현재 상대 보스 HP = %d\n",hp);
	printf("=======================================================\n");

	int status;
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while(1)
	{
		if(pthread_create(&t_id,NULL,t_func,(void *)&reads) !=0 )
		{
			perror("PrintUI_Thread create error\n");
			continue;
		}
		pthread_join(t_id, (void**)&status);
		printf("Thread End : %d\n",status);
	}
	return 0;
}

void *t_func(void *arg)
{
	int result;
	int fd_Max = 1;
	int fd_Num;
	fd_set reads = (*(fd_set*)arg);
	fd_set cpy_Reads = (*(fd_set*)arg);
	//timeout.tv_sec=5;
	//timeout.tv_usec=5000;
	char buf[256] = {0};
	int keyboard;

	while(1)
	{
		if((keyboard=select(fd_Max+1, &cpy_Reads, 0, 0, NULL))==-1)
		{
			perror("select 함수 에러 내용 : ");
			break;
		}
		if(keyboard==0)
		{
			continue;
		}

		if(FD_ISSET(0,&reads))
		{
			//handle_Connection();
		        result =select(keyboard+1,&reads,NULL,NULL,NULL);
		        switch(result)
		        {
		                case -1:
		                       	perror("error\n");
		                       	break;

		                case 0:
	                        	perror("time out, if timeout settings on\n");
	                        	break;

		                default:
        	              	  	/* Read data from stdin using fgets. */
        	              	  	fgets(buf, sizeof(buf), stdin);
        	              	  	printf("buf :%s\n",buf);
        	              	  	break;
        		}
		}
        	FILE *fp;
        	fp = popen(buf,"r");
        	if (fp == NULL)
		{
        	        perror("err\n");
        	}
		while(fgets(buf,1024,fp))
        	{
        	        printf("buf : %s",buf);
	        }
		pclose(fp);
	}
}

void SIGINT_handler(int signo, siginfo_t* si)
{
	psiginfo(si,"외부에서 SIGINT 신호가 왔습니다");
}

// 무기 변경 함수
void SIGUSR1_handler(int signo)
{
	write(STDOUT_FILENO,"무기를 변경합니다\n",26);
	switch(weaponFlag)
	{
		case 0:
			write(STDOUT_FILENO,"현재 무기는 맨주먹 입니다\n",37);
			write(STDOUT_FILENO,"현재 무기를 몽둥이로 변경합니다\n",46);
			weaponFlag = 1;
			break;
		case 1:
			write(STDOUT_FILENO,"현재 무기는 몽둥이 입니다\n",37);
			write(STDOUT_FILENO,"현재 무기를 총으로 변경합니다\n",43);
			weaponFlag = 2;
			break;
		case 2:
			write(STDOUT_FILENO,"현재 무기는 총 입니다\n",31);
			write(STDOUT_FILENO,"현재 무기를 맨주먹으로 변경합니다\n",49);
			weaponFlag = 0;
			break;
	}
}

// 공격 함수
void SIGUSR2_handler(int signo)
{
	write(STDOUT_FILENO,"보스를 공격합니다\n",26);
	switch(weaponFlag)
	{
		case 0:
			write(STDOUT_FILENO,"현재 무기는 맨주먹 입니다\n",37);
			write(STDOUT_FILENO,"맨주먹으로 보스를 공격합니다\n",42);
			hp = hp - 5;
			printf("현재 보스체력 : %d\n",hp);
			break;
		case 1:
			write(STDOUT_FILENO,"현재 무기는 몽둥이 입니다\n",37);
			write(STDOUT_FILENO,"몽둥이로 보스를 공격합니다\n",39);
			hp = hp - 10;
			printf("현재 보스체력 : %d\n",hp);
			break;
		case 2:
			write(STDOUT_FILENO,"현재 무기는 총 입니다\n",31);
			write(STDOUT_FILENO,"총으로 보스를 공격합니다\n",36);
			hp = hp - 20;
			printf("현재 보스체력 : %d\n",hp);
			break;
	}
	if (hp <= 0)
	{
		sigaction(SIGINT,&sa_old,0);
		write(STDOUT_FILENO,"이제 SIGINT 신호가 오면 프로그램이 종료됩니다\n",63);
	}
}

