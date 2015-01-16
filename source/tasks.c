#include <threadQueue.h>

void task1()
{
	int *g = (int*)0x900000;
	*g = 9153;
	unsigned int t1 = 0;
	unsigned int i = 0;
	for(; i <= 50000000; i++)
		t1 = 0;
	i = 0;
	for(; i <= 99; i++)
	{
		t1 += i;
	}

	printf("summation of 99 is %u. ", t1);
	printf("g is %u\n", *g);
}

void task2()
{
	int *g = (int*)0x900000;
	*g = 6786;

	unsigned int t2 = 0;
	unsigned int i = 0;
	for(; i <= 50000000; i++)
		t2 = 0;
	i = 0;
	for(; i <= 100; i++)
	{
		t2 += i;
	}
	
	printf("summation of 100 is %u. ", t2);
	printf("g is %u\n", *g);
}
