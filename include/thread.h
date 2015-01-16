#ifndef _thread_
#define _thread_

typedef struct _process process;
typedef struct _thread thread;
enum threadStates {
	THREAD_READY = 1,
	THREAD_RUNNING = 2,
	THREAD_DONE = 3
};

struct _thread
{
	unsigned int id;
	unsigned int eip;
	unsigned int esp;
	enum threadStates state;
	void (*func)();
	process *process;
};

#include <process.h>

void threadConstruct(thread *t, void(*func1)(), unsigned int esp);
void threadJoin(thread *t);

thread *currentThread;
#endif