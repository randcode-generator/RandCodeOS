#include <threadQueue.h>
#include <VMM.h>

void task1()
{
	int* num1 = valloc(&currentThread->process->vmm, 32);
	*num1 = 7731;
	printf("%x %u\n", num1, *num1);
	num1 = valloc(&currentThread->process->vmm, 64);
	*num1 = 1923;
	printf("%x %u\n", num1, *num1);

	int *g = (int*)0x800000;
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
	int* num1 = valloc(&currentThread->process->vmm, 32);
	*num1 = 2321;
	printf("%x %u\n", num1, *num1);
	num1 = valloc(&currentThread->process->vmm, 64);
	*num1 = 9831;
	printf("%x %u\n", num1, *num1);

	int *g = (int*)0x800000;
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
