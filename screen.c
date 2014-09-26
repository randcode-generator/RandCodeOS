unsigned int VIDEO_MEMORY = 0xB8000;
unsigned short x = 0;
unsigned short y = 0;

unsigned char SCREEN_WIDTH = 80;
unsigned char SCREEN_HEIGHT = 25;

void moveCursorTo();

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

void clrScreen()
{
	unsigned char *memory = (unsigned char *)0xB8000;
	unsigned short linear = SCREEN_WIDTH * SCREEN_HEIGHT * 2 - 10;
	unsigned short i = 0;
	for(; i < linear;)
	{
		memory[i] = ' ';
		i++;
		memory[i] = 15;
		i++;
	}
	setXY(0,0);
}
