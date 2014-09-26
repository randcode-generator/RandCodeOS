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
