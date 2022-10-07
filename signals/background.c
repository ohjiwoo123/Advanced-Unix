#include <stdio.h>
#include <unistd.h> // for fork
#include <stdlib.h> // for exit
#include <string.h>
#include <sys/stat.h> // for umask
#include <sys/types.h>
#include <syslog.h>
#include <signal.h>

int hp = 100;
int weaponFlag = 0;

void SIGINT_handler(int signo, siginfo_t* si);
void SIGUSR1_handler(int signo);
void SIGUSR2_handler(int signo);

struct sigaction sa_old; /* Old signal actions */
struct sigaction sa_new; /* New signal actions */

int main()
{
	sa_new.sa_handler = SIGINT_handler; /* Point to our function */
        sigemptyset(&sa_new.sa_mask); /* Clear mask */
        sa_new.sa_flags = SA_SIGINFO; /* No special flags */

        sigaction(SIGINT,&sa_new,&sa_old);
        signal(SIGUSR1,&SIGUSR1_handler);
        signal(SIGUSR2,&SIGUSR2_handler);

	pid_t pid;
	int x_fd;

	// STEP-1 FORK OFF THE PARENT PROCESS
	pid = fork();
	if (pid <0)
	{
		exit(EXIT_FAILURE);
	}

	// STEP-2 THE CHILD PROCESS 
	if(setsid() < 0)
		exit(EXIT_FAILURE);

	// STEP-3 CATCH, IGNORE AND HANDLE SIGNALS
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	// STEP-4 FORK OFF FOR THE SECOND TIME
	pid = fork();
	if(pid<0)
	{
		exit(EXIT_FAILURE);
	}
	// IF SUCCESS : LET THE PARENT TERMINATE
	if(pid>0)
	{
		printf("Parent pid =%d\n", getpid());
		exit(EXIT_SUCCESS);
	}

	// STEP-5 SET THE NEW FILE PERMISSONS THAT IS CREATED BY DAEMON
	umask(077);

	// STEP-6 change the current working directory
	chdir("/");

	// STEP-7
	// close all open file descriptors
	for(x_fd = sysconf(_SC_OPEN_MAX); x_fd>=0; x_fd--)
	{
		if(x_fd==1)
			continue;
		close(x_fd);
	}

	int result;
	char buf[256] = {0};

	// STEP-8
	// LOGGING ERRORS / INFO IN THE SYSLOG SYSTEM
	int count = 0;
	openlog("Logs", LOG_PID, LOG_USER);
	while(1)
	{
		sleep(1);
		//syslog(LOG_INFO, LOGGING, count++);
	}
	closelog();
	return 1;

}

void SIGINT_handler(int signo, siginfo_t* si)
{
        psiginfo(si,"외부에서 SIGINT 신호가 왔습니다");
        write(STDOUT_FILENO,"보스 hp가 0이 되기 전까진 SIGINT 신호로 게임종료가 불가능합니다\n",87);
        return;
}

// 무기 변경 함수
void SIGUSR1_handler(int signo)
{
        if(hp <=0)
        {
                write(STDOUT_FILENO,"게임이 종료되었습니다, SIGINT 신호를 날리세요\n",63);
                return;
        }
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
        return;
}

// 공격 함수
void SIGUSR2_handler(int signo)
{
        if(hp <=0)
        {
                write(STDOUT_FILENO,"보스가 죽었습니다(게임 끝), SIGINT 신호를 날리세요\n",69);
                return;
        }
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
        return;
}
