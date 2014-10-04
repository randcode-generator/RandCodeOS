unsigned char inportb(unsigned short port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0":"=a"(rv): "Nd"(port));
	return rv;
}

void outportb(unsigned short port, unsigned char command)
{
	__asm__ __volatile__ ("outb %1, %0"::"d"(port), "a"(command));
}

void memcpy(void* dest, void* src, unsigned int size)
{
	unsigned char* dest1 = (unsigned char*)dest;
	const unsigned char* src1 = (unsigned char*)src;

	while(size--)
	{
		*dest1++ = *src1++;
	}
}

void memset(void* src, unsigned char value, unsigned int size)
{
	unsigned char* src1 = (unsigned char*)src;
	while(size--)
	{
		*src1++ = value;
	}
}