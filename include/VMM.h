#ifndef _VMM_
#define _VMM_

typedef struct _VMM VMM;

struct node {
	unsigned char type;
	unsigned int start;
	unsigned int size;
	struct node *prev;
	struct node *next;
};

struct _VMM {
	struct node* globaln;
	struct node* head;
};

void VMM_init(VMM* vmm);
void* valloc(VMM* vmm, int size);
#endif