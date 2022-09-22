#include <stdio.h>

static int hp = 100;

int main()
{
	while(1)
	{
		Minus();
		sleep(1);
		if(hp<=0)
		{
			return 0;
		}
	}

}

void Minus()
{
	hp = hp-20;
	printf("hp : %d\n",hp);

}
