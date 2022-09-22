/* rsig1.c */
// ctrl -c 10번하 game clear
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static int hp = 100; 
int flag = 0;

struct sigaction sa_old; /* Old signal actions */
struct sigaction sa_new; /* New signal actions */
void h_attckWithGun(int signo);
void h_attackWithKnife(int signo);

void h_attackWithGun(int signo)
{
    //if(flag==1)
    //{
    	//signal(SIGINT,h_attackWithKnife);
	//return;
    //}
    printf("무기로 총이 쥐어집니다. (공격력 : 20)\n");
    printf("총으로 공격!\n");
    hp = hp-20;
    printf("현재 상대 보스 체력 : %d\n",hp);
    if( hp<=50)
    {
    	signal(SIGINT,h_attackWithKnife);
    }
}

void h_attackWithKnife(int signo)
{
    if(flag==0)
    {
        // 시그널 번호와 시그널이 들어오면 시그널을 처리할 함수 
        signal(SIGINT,h_attackWithGun); /* Re-instate handler */
        printf("이제 무기가 총으로 바뀝니다.\n");
    }
    if(flag==1)
    {
	signal(SIGINT,h_attackWithKnife);
    }
    printf("무기로 몽둥이가 쥐어집니다. (공격력 : 10)\n");
    printf("몽둥이로 공격!\n");
    hp = hp-10;
    printf("현재 상대 보스 체력 : %d\n",hp);
}

int main(int argc,char **argv)
{
    sa_new.sa_handler = h_attackWithKnife; /* Point to our function */
    sigemptyset(&sa_new.sa_mask); /* Clear mask */
    sa_new.sa_flags = 0; /* No special flags */

    sigaction(SIGINT,&sa_new,&sa_old);

    printf("시그널을 활용한 상대 보스를 죽이는 게임입니다.\n");
    printf("현재 상대 보스 HP = %d\n",hp);

    while (hp > 50)
    {
        printf("상대 보스를 공격하려면 CTRL-C를 누르세요.\n");
        sleep(4);
    }

    while (hp <= 50)
    {
            flag=1;
            printf("보스체력이 (50)절반 이하입니다.\n");
            printf("CTRL-C를 누르면 이제 무기가 몽둥이로 고정됩니다.\n");
            printf("상대 보스를 공격하려면 CTRL-C를 누르세요.\n");
            printf("hp %d\n",hp);
            sleep(4); /* Snooze */

	    if(hp<=0)
	    {
	        printf("보스 체력이 0입니다. 당신의 승리입니다.\n");
		printf("이제 CTRL-C를 한번 더 누르면 프로그램이 종료됩니다.\n");
		sigaction(SIGINT,&sa_old,0);
	    }
    }

    puts("End.\n");
    return 0;
}
