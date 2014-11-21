#ifndef _thread_
#define _thread_

typedef struct _process process;
typedef struct _thread thread;
struct _thread
{
	unsigned int id;
	unsigned int eip;
	unsigned int esp;
	unsigned int state;
	void (*func)();
	process *process;
};

#include <process.h>

void threadConstruct(thread *t, void(*func1)(), unsigned int esp);
void threadJoin(thread *t);
thread* getFreeThread();

thread *currentThread;

thread threads[10];
#endif