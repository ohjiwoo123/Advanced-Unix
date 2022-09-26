#include <stdio.h>
#include <assert.h>
#include <sys/select.h>
#include <fcntl.h>
int main ()
{
    int keyboard;
    int ret,i;
    char c;
    fd_set readfd;
    struct timeval timeout;
    keyboard = open("/dev/tty",O_RDONLY | O_NONBLOCK);
    printf("keyboard = %d\n",keyboard);
    assert(keyboard>0);
    while(1)
    {
        //timeout.tv_sec=1;
        //timeout.tv_usec=0;
        FD_ZERO(&readfd);
        FD_SET(keyboard,&readfd);

	char buff[256] = {0};
        ret=select(keyboard+1,&readfd,NULL,NULL,NULL);
        switch(ret)
	{
		case -1:
			perror("error\n");
			break;
		case 0:
			perror("0\n");
			break;
		default:
			/* Read data from stdin using fgets. */
        		fgets(buff, sizeof(buff), stdin);
			printf("buff :%s\n",buff);
			break;
	}
	FILE *fp;
	fp = popen(buff,"r");
	if (fp == NULL)
		perror("err\n");
	while(fgets(buff,1024,fp))
	{
		printf("buff : %s\n",buff);
	}
    }
}
