#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

void daemonize(const char *cmd){
        int i,fd0,fd1,fd2;
        pid_t pid;
        struct rlimit rl;
        struct sigaction sa;

        //umask를 0으로
        umask(0);

        if(getrlimit(RLIMIT_NOFILE,&rl)<0){
                printf("%s: can't get file limit\n",cmd);
                exit(0);
        }

        //자식 프로세스 생성 후 부모는 종료
        if((pid=fork())<0){
                printf("%s: can't fork\n",cmd);
                exit(0);
        }else if(pid !=0)
                exit(0);

        //세션을 만들고 리더가 됨.
        setsid();

        //SIGHUP 무시, 제어터미널 연결이 끊겨도 무시
        sa.sa_handler=SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags=0;
        if(sigaction(SIGHUP,&sa,NULL)<0){
                printf("%s: can't ignore SIGHUP\n",cmd);
                exit(0);
        }
        if((pid=fork())<0){
                printf("%s:can't fork\n",cmd);
                exit(0);
        }
        else if(pid!=0)
                exit(0);


        // 작업 디렉토리를 /로 변경
        if(chdir("/") < 0){
                printf("%s: can't change directory to /\n",cmd);
                exit(0);
        }

        //열려있는 파일 디스크립터 닫기
        if(rl.rlim_max == RLIM_INFINITY)
                rl.rlim_max = 1024;
        for(i=0;i<rl.rlim_max;i++)
                        close(i);

        //모두 닫았으니 fd는 0부터 return되며 모두 /dev/null로 직행
        fd0=open("/dev/null",O_RDWR);
        fd1=dup(0);
        fd2=dup(0);

        openlog(cmd,LOG_CONS,LOG_DAEMON);
        if(fd0 != 0 || fd1 != 1 || fd2 != 2){
                syslog(LOG_LOCAL0," unexpected fd : %d %d %d",fd0,fd1,fd2);
                exit(1);
        }

        syslog(LOG_LOCAL0,"helloworld! [%s]start",cmd);
}


int main(int argc,char* argv[]){
        daemonize(argv[0]);

        //100초후 데몬 종료
        sleep(100);
}
