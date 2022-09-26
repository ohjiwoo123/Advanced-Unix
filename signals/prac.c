#include <stdio.h>
#include <signal.h>

int main()
{

	sigset_t myset;
	sigemptyset(&myset);
	sigaddset(&myset,SIGTERM);

	while(1)
	{
		printf("test\n");
		sleep(1);
	}
	return 0;
}

