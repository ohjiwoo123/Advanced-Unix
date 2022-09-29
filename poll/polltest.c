#include <stdio.h>
#include <signal.h>
#include <sys/poll.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct pollfd fds;
	int result = 0;

	char buf[255+1];
	int len;

	fds.fd = STDIN_FILENO;
	fds.events = POLLIN;



	while(1)
	{
		result = poll(&fds,1,3500);

		if (result == -1)
		{
			perror("-1 err\n");
		}
		else if (result == 0)
		{
			printf("timeout\n");
		}
		if (fds.revents & POLLIN)
		{
			result = read(STDIN_FILENO,buf,1024);
			if (result == -1)
			{
				perror("read err\n");
			}
			if (result)
			{
				printf("read : buf %s\n",buf);
			}

		}
	}
	return 0;
}
