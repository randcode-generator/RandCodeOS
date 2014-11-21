unsigned int address = 0;
unsigned int physicalMemoryManagerGetPhysicalMemory(unsigned int size)
{
	unsigned int tmpAddress = address;
	address += size;
	return tmpAddress;
}
