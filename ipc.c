#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>


int main()
{
int msgget(key_t key, int flags);
int mqid; /* Message Queue IPC ID */
mqid = msgget(0xFEEDBEEF,IPC_CREAT|0600);
printf("mqid : %d\n",mqid);
return 0;
}
