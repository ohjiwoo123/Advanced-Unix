#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main( void)
{
   sigset_t sigset;
   sigset_t pendingset;
   int      ndx = 0;

   // 모든 시그널을 블록화
   sigfillset( &sigset);
   sigprocmask(SIG_SETMASK, &sigset, NULL);

   while( 1 ){
      printf( "카운트: %d\n", ndx++);
      sleep( 1);
      if ( 0 == sigpending( &pendingset)){
	 //sigdelset(&pendingset,SIGINT);
         if ( sigismember( &pendingset, SIGINT)){
            printf( "Ctrl-C를 누르셨죠. 무한 루프를 종료합니다.\n");
            break;
         }
      }
   }
   return 0;
}
