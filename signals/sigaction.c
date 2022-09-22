/* rsig1.c */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static int count = 0;

void h2(int signo)
{
    count += 5; /* Increment count */
    write(1,"Got SIGINT2\n",11); /* Write message */
}

void h1(int signo)
{
    // 시그널 번호와 시그널이 들어오면 시그널을 처리할 함수 
    signal(SIGINT,h2); /* Re-instate handler */
    ++count; /* Increment count */
    printf("siganlHandler 함수 입니다\n");
    write(1,"Got SIGINT\n",11); /* Write message */
}

int main(int argc,char **argv)
{
    struct sigaction sa_old; /* Old signal actions */
    struct sigaction sa_new; /* New signal actions */

    sa_new.sa_handler = h1; /* Point to our function */
    sigemptyset(&sa_new.sa_mask); /* Clear mask */
    sa_new.sa_flags = 0; /* No special flags */
    sigaction(SIGINT,&sa_new,&sa_old);

    while ( count < 6 )
    {
        puts("Waiting for SIGINT..");
        sleep(4); /* Snooze */
    }

    sigaction(SIGINT,&sa_old,0); /* Restore signal actions */
    printf("시그액션함수의 핸들러 = %s\n",&sa_old.sa_handler);
    while ( count < 8 )
    {
        puts("Waiting for SIGINT2..");
        sleep(4); /* Snooze */
    }

    puts("End.");
    return 0;
} 
