#include <stdio.h>
#include <unistd.h> // for fork
#include <stdlib.h> // for exit
#include <string.h>
#include <sys/stat.h> // for umask
#include <sys/types.h>
#include <syslog.h>
#include <signal.h>

#define LOGGING "Start Logging my task =%d\n"

int main()
{
	pid_t pid;
	int x_fd;

	// STEP-1 FORK OFF THE PARENT PROCESS
	pid = fork();
	if (pid <0)
	{
		exit(EXIT_FAILURE);
	}

	// STEP-2 THE CHILD PROCESS 
	if(setsid() < 0)
		exit(EXIT_FAILURE);

	// STEP-3 CATCH, IGNORE AND HANDLE SIGNALS
	signal(SIGCHLD, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	// STEP-4 FORK OFF FOR THE SECOND TIME
	pid = fork();
	if(pid<0)
	{
		exit(EXIT_FAILURE);
	}
	// IF SUCCESS : LET THE PARENT TERMINATE
	if(pid>0)
	{
		printf("Parent pid =%d\n", getpid());
		exit(EXIT_SUCCESS);
	}

	// STEP-5 SET THE NEW FILE PERMISSONS THAT IS CREATED BY DAEMON
	umask(077);

	// STEP-6 change the current working directory
	chdir("/");

	// STEP-7
	// close all open file descriptors
	for(x_fd = sysconf(_SC_OPEN_MAX); x_fd>=0; x_fd--)
	{
		close(x_fd);
	}

	// STEP-8
	// LOGGING ERRORS / INFO IN THE SYSLOG SYSTEM
	int count = 0;
	openlog("Logs", LOG_PID, LOG_USER);
	while(1)
	{
		sleep(2);
		syslog(LOG_INFO, LOGGING, count++);
	}
	closelog();
	return 1;

}
