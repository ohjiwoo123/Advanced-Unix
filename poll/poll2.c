#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/poll.h>

void quit(int rc,const char *fmt,...)
{
    va_list ap;
    if ( errno != 0 ) /* Report errno */
    {
        fprintf(stderr,"%s: ",strerror(errno));
    }
    va_start(ap,fmt); /* Format error message */
    vfprintf(stderr,fmt,ap);
    va_end(ap);
    fputc('\n',stderr);
    exit(rc); /* Exit with return code */
}

int main(int argc,char **argv)
{
    struct pollfd fds[2]; /* Poll events */
    int z = 0; /* General status code */
    int f1 = 0; /* Open fifo 1 */
    int f2 = 0; /* Open fifo 2 */
    memset(fds,0,sizeof(fds));
    int nfds = 0; /* Number of file descriptors */
    char buf[200+1]; /* I/O Buffer */
    FILE *p1, *p2; /* Pipes from popen(3) */
    memset(buf,0,sizeof(buf));
    /*
    * Pipes :
    */
    if ( !(p1 = popen("ls -l|tr [:lower:] [:upper:]","r")) )
    {
        quit(1,"popen(3) failed for p1");
    }
    if ( !(p2 = popen("ls -l|tr [:upper:] [:lower:] && sleep 8","r")) )
    {
        quit(1,"popen(3) failed for p2");
    }

    /*
    * Obtain the underlying file descriptors :
    */
    f1 = fileno(p1);
    fds[0].fd = f1; /* File descriptor to poll.. */
    fds[0].events = POLLIN;

    f2 = fileno(p2);
    fds[1].fd = f2; /* File descriptor to poll.. */
    fds[1].events = POLLIN; /* for input events */

    nfds = 2; /* nfds is fds[2] array size */

    printf("BEGUN: f1=%d, f2=%d\n",f1,f2);

    /*
    * Enter a poll loop :
    */
    do
    {
        do
        {
            z = poll(fds,(unsigned int)2,3500); /* Timeout is 3.5 seconds */
        } while ( z == -1 && errno == EINTR );

        if ( z == -1 ) /* Error? */
        {
            quit(13,"poll(2)");
        }
        if ( z == 0 )
        {
            printf("TIMEOUT: f1=%d, f2=%d\n",f1,f2);
            continue;
        }

        /*
        * Control is here if f1 or f2 has data
        * available to be read.
        */
	if(fds[0].revents != 0)
        {
                printf("fd=%d, events: %s%s%s\n", fds[0].fd, (fds[0].revents & POLLIN) ? "POLLIN" :"", (fds[0].revents & POLLHUP) ? "POLLHUP" : "", (fds[0].revents & POLLERR) ? "POLLERR" : "");
        }

        if (fds[0].revents & POLLIN || fds[0].revents & POLLHUP)
        {
            z = read(f1,buf,sizeof buf-1);
            if ( z == -1 )
            {
                quit(6,"read(2) of f1.");
            }
            if ( z > 0 )
            {
                buf[z] = 0;
                printf("*** read %d bytes <<<%s>>> from f1\n",z,buf);
            }
            else
            {
                puts("read EOF from f1;");
                pclose(p1);
                fds[0].fd = f1 = -1;
            }
        }

	if(fds[1].revents != 0)
        {
                printf("fd=%d, events: %s%s%s\n", fds[1].fd, (fds[1].revents & POLLIN) ? "POLLIN" :"", (fds[1].revents & POLLHUP) ? "POLLHUP" : "", (fds[1].revents & POLLERR) ? "POLLERR" : "");
        }

        if (fds[1].revents & POLLIN || fds[1].revents & POLLHUP)
        {
            z = read(f2,buf,sizeof buf-1);
            if ( z == -1 )
            {
                quit(6,"read(2) of f2.");
            }
            if ( z > 0 )
            {
                buf[z] = 0;
                printf("*** read %d bytes <<<%s>>> from f2\n",z,buf);
            }
            else
            {
                puts("read EOF from f2;");
                pclose(p2);
                fds[1].fd = f2 = -1;
            }
        }
    } while ( f1 >= 0 || f2 >= 0 );

    puts("End poll.");

    return 0;
}
