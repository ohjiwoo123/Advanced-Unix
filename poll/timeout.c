#include <stdio.h>
#include <poll.h>

int main(int argc, char *argv[])
{
	int f1 = 1;
	int nfds = 1;
	int result;

	struct pollfd fds;
	fds.fd = 1;
	fds.events = POLLIN;
	char buf[1024];

	while(1)
	{
		result = poll(fds.fd,nfds,3500);
		if (result == -1)
		{
			perror("err\n");
		}
		else if (result == 0)
		{
			printf("TIMEOUT\n");
			continue;
		}
		if(fds.revents & POLLIN)
		{
			result = read(f1,buf,sizeof(buf));
			if(result == -1)
			{
				perror("read err\n");
			}
			else if (result > 0)
			{
				while(fgets(buf,sizeof(buf),stdin))
				{
					printf("%s\n",buf);
				}
			}
			else
			{
				printf("read EOF from f1");
				//pclose(p1);
			}
		}
	}
	return 0;
}
