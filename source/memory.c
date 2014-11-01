unsigned int *paging_directory=(unsigned int*)0x9C000;
unsigned int *paging_entry=(unsigned int*)0x9D000;

//interrupt 14
void paging_exception(unsigned int error)
{
	printf("Paging, error:%u, ", error);
	unsigned int fault_address;
	asm("mov %%cr2, %0":"=r"(fault_address));
	printf("0x%x, ", fault_address);
	printf("%s, ",(error & 1) == 0 ? "not present" : "protection violation");
	printf("%s, ",(error & 2) >> 1 == 0 ? "read" : "write");
	printf("%s\n",(error & 4) >> 2 == 0 ? "supervisor" : "user mode");

	asm("mov %%cr2, %0":"=r"(fault_address));
	unsigned int offset = fault_address & 0xFFF;
	unsigned int page = (fault_address & 0x3FF000) >> 12;
	unsigned int dir = (fault_address & 0xFFC00000) >> 22;

	printf("%u %u %u\n", dir, page, offset);

	unsigned address = 4096 * (1024 * dir + page);
	paging_entry[(1024*dir)+page]=address|3;
	paging_directory[dir]=(unsigned int)(paging_entry+(1024*dir));
	paging_directory[dir]=paging_directory[dir]|3;
}

void setup_paging()
{
	int address=0;
	int i = 0;
	for(i = 0; i < 1024; i++)
		paging_directory[i] = 0;
	for(i = 0; i < 1024; i++)
	{
		paging_entry[i]=0;
		paging_entry[i]=address|3;
		address+=4096;
	}
	paging_directory[0]=(unsigned int)paging_entry;
	paging_directory[0]=paging_directory[0]|3;

	asm volatile("mov %0, %%cr3":: "r"(paging_directory));
	unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}