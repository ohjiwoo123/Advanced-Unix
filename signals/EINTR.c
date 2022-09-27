#include <stdio.h>
#include <signal.h>
#include <errno.h>

int gotSIGINT = 0; /* True when SIGINT arrives */
int count = 0;

void handler(int signo);
void catch_SIGINT(int signo);

int main(int argc, char *argv[])
{
	signal(SIGINT,catch_SIGINT);
	int z;
	char buf[65000];

	do
	{
		z = read(0,buf,sizeof(buf));
		if (gotSIGINT)
			handler(SIGINT);
	} while ( z == -1 && errno == EINTR);

	printf("z=%d\n",z);
	printf("errno=%d\n",errno);
	return 0;
}


void handler(int signo)
{
	int e = errno; /* Save errno */
	++count; /* Increment count */
	write(1,"Got SIGINT\n",11); /* Write message */
	errno = e; /* Restore errno */
}

void catch_SIGINT(int signo)
{
	gotSIGINT = 1;
}
