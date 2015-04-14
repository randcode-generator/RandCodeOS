#ifndef _process_
#define _process_

#include <VMM.h>
#include <thread.h>
typedef struct _process process;

enum processStates {
	PROCESS_READY,
	PROCESS_RUNNING
};

struct _process 
{
	unsigned int id;
	unsigned short threadCount;
	unsigned int cr3;
	enum processStates state;
	VMM vmm;
	thread threads[3];
};

void processConstruct(process *p, void(*func1)(), unsigned int cr3);
void processAddThread(process *p, thread *t);
thread *processGetFreeThread(process *p);
process *getFreeProcess();

process processes[10];
#endif