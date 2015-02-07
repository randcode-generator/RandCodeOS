/*
	Physical Memory Manager
*/

unsigned int address = 0x400000;
struct memNode {
	int index;
	unsigned int arr;
	struct memNode *next;
};

struct memNode *root = (struct memNode*)0x300000;

unsigned int palloc(struct memNode *n) {
	while(1) {
		if(n->arr == 0xffffffff && n->next == 0) {
			struct memNode *m = (struct memNode*)(((n->index + 1) * sizeof root) + n);
			m->index = n->index + 1;
			m->arr = 1;
			n->next = m;
			unsigned int physAddress = (m->index * 32 * 4096);
			return physAddress;
		}
		else if(n->arr != 0xffffffff) {
			unsigned int arr = n->arr;
			int mask = 1;
			int i = 0;
			for(; i < 32; i++) {
				if((arr & 1) == 0) {
					unsigned int physAddress = (n->index * 32 * 4096) + (i * 4096);
					n->arr = n->arr | mask;
					return physAddress;
				}
				arr = arr >> 1;
				mask = mask << 1;
			}
		}
		n = n->next;
	}
	return 0;
}

void pfree(unsigned int mem) {
	unsigned int t = mem / 4096;
	unsigned int index = t / 32;
	unsigned int offset = t % 32;

	struct memNode *n = root;
	int i = 0;
	while(1) {
		if(i == index) {
			unsigned int mask = ~(1 << offset);
			n->arr = n->arr & mask;
			break;
		}
		n = n->next;
		i++;
	}
}

void initialize_PMM() {
	root->index = 0;
	root->arr = 0;
	root->next = 0;
	int i = 0;
	for(; i < 1024; i++) {
		palloc(root);
	}
}

unsigned int PMM_GetPhysicalMemory()
{
	unsigned int g = palloc(root);
	return g;
}

unsigned int page_table = 0x103000;
unsigned int PMM_getPageTable() {
	unsigned int tmpAddress = page_table;
	page_table += 4096;
	return tmpAddress;
}