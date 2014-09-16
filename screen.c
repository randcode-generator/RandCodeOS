unsigned int VIDEO_MEMORY = 0xB8000;
unsigned short x = 0;
unsigned short y = 0;

void moveCursorTo();
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

void setXY(unsigned short xArg, unsigned short yArg)
{
	x = xArg;
	y = yArg;
	moveCursorTo();
}

void moveCursorTo()
{
	VIDEO_MEMORY = 0xB8000 + ((y * 80) + x) * 2;
	unsigned short position = y * 80 + x;
	outportb(0x3D4, 14);
	outportb(0x3D5, position >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, position & 0xFF);
}

void incrementCursor()
{
	x++;
	if(x >= 80)
	{
		x = 0;
		y++;
	}
	moveCursorTo();
}

void putchar(char c)
{
	unsigned char* memory = (unsigned char*)VIDEO_MEMORY;
	*memory = c;
	memory++;
	*memory = 15;
	memory++;

	VIDEO_MEMORY = (unsigned int)memory;
	incrementCursor();
}

void printStr(const char * msg)
{
	unsigned char *memory = (unsigned char *)VIDEO_MEMORY;
	unsigned short i = 0;
	char c = ' ';
	while(1)
	{
		c = *(msg+i);
		if(c == 0)
			break;
		putchar(c);
		i++;
	}
}
