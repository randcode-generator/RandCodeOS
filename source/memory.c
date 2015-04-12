void allocate_memory(unsigned int fault_address, unsigned int cr3);

//interrupt 14
void paging_exception(unsigned int error)
{
	unsigned int fault_address;
	asm("mov %%cr2, %0":"=r"(fault_address));
	
	unsigned int cr3Value;
	asm("mov %%cr3, %0":"=r"(cr3Value));
	
	allocate_memory(fault_address, cr3Value);
}

void initialize_paging(unsigned int m)
{
	unsigned int *paging_directory=(unsigned int*)m;
	unsigned int *paging_entry=(unsigned int*)(m + 0x1000);
	int address=0;
	int i = 0;
	for(i = 0; i < 1024; i++)
		paging_directory[i] = 0;
	for(i = 0; i < 1024; i++)
	{
		paging_entry[i]=0;
		paging_entry[i]=address|3;
		address += 4096;
	}
	paging_directory[0]=(unsigned int)paging_entry;
	paging_directory[0]=paging_directory[0]|3;

	asm volatile("mov %0, %%cr3":: "r"(paging_directory));
}

void setup_kernel_space(unsigned int m)
{
	unsigned int *paging_directory=(unsigned int*)m;
	unsigned int *paging_entry=(unsigned int*)(m + 0x1000);
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
}

void enable_paging()
{
	unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void allocate_memory(unsigned int fault_address, unsigned int cr3)
{
	unsigned int dir = fault_address >> 22;
	unsigned int table = (fault_address & 0x003FF000) >> 12;

	unsigned int cr3Value = cr3;	
	unsigned int *paging_directory = (unsigned int*)cr3Value;
	unsigned int *paging_entry = 0;

	if ((paging_directory[dir] & 1) == 1)
	{
		paging_entry = (unsigned int *)(paging_directory[dir] & 0xFFFFF000);
	}
	else {
		paging_entry = (unsigned int*)PMM_getPageTable();
		paging_directory[dir] = (unsigned int)&paging_entry[0];
		paging_directory[dir] |= 0x3;
		int i = 0;
		for(; i < 1024; i++)
			paging_entry[i] = 0;
	}
	unsigned int address = 0;
	address = PMM_GetPhysicalMemory();
	paging_entry[table] = 0;
	paging_entry[table] = address | 3;
}