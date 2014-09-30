extern void memcpy(void* dest, void* src, unsigned int size);
unsigned int VIDEO_MEMORY = 0xB8000;
unsigned short position = 0;

unsigned char SCREEN_WIDTH = 80;
unsigned char SCREEN_HEIGHT = 25;

void moveCursorTo();
void clrScreen();
void putch(char c);

void setXY(unsigned short x, unsigned short y)
{
	position = y * SCREEN_WIDTH + x;
	moveCursorTo();
}

void getXY(unsigned short *x, unsigned short *y)
{
	*x = position % SCREEN_WIDTH;
	*y = position / SCREEN_WIDTH;
}

void moveCursorTo()
{
	VIDEO_MEMORY = 0xB8000 + position * 2;
	outportb(0x3D4, 14);
	outportb(0x3D5, position >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, position & 0xFF);
}

void scrollScreen()
{
	memcpy((unsigned int*)0xB8000, (unsigned int*)0xB80A0,
		SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2);
	setXY(0, SCREEN_HEIGHT - 1);
	int i = 0;
	for(; i < SCREEN_WIDTH; i++)
	{
		putch(' ');
	}
	setXY(0, SCREEN_HEIGHT - 1);
}

void nextLine()
{
	unsigned short x, y;
	getXY(&x, &y);
	setXY(0, y + 1);
	if(position > SCREEN_WIDTH * SCREEN_HEIGHT - 1)
		scrollScreen();
}

void deleteChar()
{
	if(position == 0)
		return;
	position--;
	moveCursorTo();
	unsigned char* memory = (unsigned char*)VIDEO_MEMORY;
	*memory = ' ';
	*(memory + 1) = 15;
}

void incrementCursor()
{
	position++;
	if(position > SCREEN_WIDTH * SCREEN_HEIGHT - 1)
		scrollScreen();
	else
		moveCursorTo();
}

void putch(char c)
{
	unsigned char* memory = (unsigned char*)VIDEO_MEMORY;
	*memory = c;
	memory++;
	*memory = 15;
	memory++;

	VIDEO_MEMORY = (unsigned int)memory;
}

void putchar(char c)
{
	putch(c);
	incrementCursor();
}

void clrScreen()
{
	unsigned char *memory = (unsigned char *)0xB8000;
	unsigned short linear = SCREEN_WIDTH * SCREEN_HEIGHT * 2;
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

const char HexNum[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void printNumber(unsigned int i, unsigned short radix)
{
	unsigned short counter = 0;
	unsigned char arr[20];
	do {
		arr[counter] = HexNum[i % radix];
		counter++;
		i /= radix;
		if(counter >= 20)
			break;
	} while(i != 0);

	arr[counter] = 0;
	while(counter != 0)
	{
		putchar(arr[--counter]);
	}
}

void printString(const unsigned char * msg)
{
	unsigned short int i = 0;
	unsigned char c = ' ';
	while((c = msg[i++])!=0)
	{
		putchar(c);
	}
}

void printf(const char * msg, ...)
{
	unsigned char *memory = (unsigned char *)VIDEO_MEMORY;
	unsigned short i = 0;
	char c = ' ';

	unsigned char *p = (char*)&msg + sizeof(char*);
	while(1)
	{
		c = *(msg+(i++));
		if(c == '%')
		{
			char specifier = *(msg+(i++));
			switch(specifier)
			{
				case 'c':
					c = (unsigned char)*((int*)p);
					p += sizeof(int);
				break;
				case 'u':
					printNumber((unsigned int)*((int*)p), 10);
					p += sizeof(int);
					continue;
				case 'x':
					printNumber((unsigned int)*((int*)p), 16);
					p += sizeof(int);
					continue;
				case 's':
					printString((const unsigned char*)*((int*)p));
					p += sizeof(int);
					continue;
				default:
				break;
			}
		}
		else if(c == '\n')
		{
			nextLine();
			continue;
		}
		if(c == 0)
			break;
		putch(c);
		incrementCursor();
	}
}