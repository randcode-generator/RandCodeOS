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

struct VMM {
	struct node* globaln;
	struct node* head;
};

//struct node* globaln = (struct node*)0x700000;
//struct node* head = 0;

struct node *findNode(struct VMM *vmm) {
	int i;
	for (i = 0; i < 10; i++)
	{
		if(vmm->globaln[i].type == NONE)
			return &vmm->globaln[i];
	}
	return 0;
}

struct node *findFreeNode(struct VMM* vmm, int size) {
	struct node *n = vmm->head;
	while(n != 0) {
		if (n->type == FREE && n->size >= size)
		{
			return n;
		}
		n = n->next;
	}
	return 0;
}

int valloc(struct VMM* vmm, int size) {
	struct node *n = 0;
	struct node *free1 = findFreeNode(vmm, size);

	if(free1->size == size) {
		n = free1;
		n->type = ALLOC;
	}
	else {
		n = findNode(vmm);
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

	if (vmm->head == free1)
	{
		vmm->head = n;
	}
	return n->start;
}

void vfree(struct VMM* vmm, int t) {
	struct node *n = vmm->head;
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

void VMM_init(struct VMM* vmm) {
	if(vmm->head != 0)
		return;
	vmm->head = vmm->globaln;
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		vmm->globaln[i].type = NONE;
	}

	vmm->globaln[0].type = FREE;
	vmm->globaln[0].start = 0x900000;
	vmm->globaln[0].size = 1000;
	vmm->globaln[0].next = 0;
	vmm->globaln[0].prev = 0;
	printf("init\n");
}