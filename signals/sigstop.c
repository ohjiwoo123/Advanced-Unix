#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
void stopHandler(int sig){
        printf("this program will stop in 3 seconds..\n");
        sleep(3);
        exit(0);
}

int main(){
        signal(SIGKILL, stopHandler);
        printf("input Ctrl+Z\n");
        while(1);
}
