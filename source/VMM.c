#define NONE 0
#define ALLOC 1
#define FREE 2

struct node {
	unsigned char type;
	unsigned int start;
	unsigned int size;
	struct node *prev;
	struct node *next;
};

struct node* globaln = (struct node*)0x700000;
struct node *head;

struct node *findNode() {
	int i;
	for (i = 0; i < 10; i++)
	{
		if(globaln[i].type == NONE)
			return &globaln[i];
	}
	return 0;
}

struct node *findFreeNode(int size) {
	struct node *n = head;
	while(n != 0) {
		if (n->type == FREE && n->size >= size)
		{
			return n;
		}
		n = n->next;
	}
	return 0;
}

int valloc(int size) {
	struct node *n = 0;
	struct node *free1 = findFreeNode(size);

	if(free1->size == size) {
		n = free1;
		n->type = ALLOC;
	}
	else {
		n = findNode();
		//link up nodes
		if(free1->prev != 0)
			free1->prev->next = n;
		n->next = free1;
		n->prev = free1->prev;
		free1->prev = n;

		unsigned int start = free1->start;
		n->type = ALLOC;
		n->start = start;
		n->size = size;

		free1->start = free1->start + size;
		free1->size = free1->size - size;
	}

	if (head == free1)
	{
		head = n;
	}
	return n->start;
}

void vfree(int t) {
	struct node *n = head;
	while(n != 0) {
		if(n->start == t) {
			n->type = FREE;

			if(n->next != 0 && n->next->type == FREE) {
				struct node *f = n->next;
				//linkup
				f->prev = n->prev;
				n->prev->next = f;

				f->start = f->start - n->size;
				f->size = f->size + n->size;

				n->type = NONE;
				n->start = n->size = 0;
				n->next = n->prev = 0;
			}
			if(n->prev != 0 && n->prev->type == FREE) {
				struct node *f = n->prev;
				//linkup
				f->next = n->next;
				n->next->prev = f;

				f->size = f->size + n->size;				

				n->type = NONE;
				n->start = n->size = 0;
				n->next = n->prev = 0;
			}
		}
		n = n->next;
	}
}

void VMM_init() {
	head = globaln;
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		globaln[i].type = NONE;
	}

	globaln[0].type = FREE;
	globaln[0].start = 0;
	globaln[0].size = 1000;
	globaln[0].next = 0;
	globaln[0].prev = 0;
}