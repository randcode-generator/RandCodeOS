#ifndef _thread_
#define _thread_
typedef struct 
{
	unsigned int eip;
	unsigned int esp;
	unsigned int state;
	void (*func)();
}thread;

void threadConstruct(thread *t, void(*func1)(), unsigned int esp);
void threadJoin(thread *t);

thread *currentThread;

thread threads[3];
#endif