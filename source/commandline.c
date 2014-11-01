#include <tasks.h>
#define key_backspace 0x8
#define key_enter     0xD

void commandline()
{
	const char *command = "Command> ";
	printf(command);
	unsigned short bufSize = 120;
	unsigned char buf[bufSize];
	unsigned short index = 0;
	while(1)
	{
		unsigned char c = getChar();
		if(c == key_backspace)
		{
			if(index > 0)
			{
				deleteChar();
				buf[--index] = 0;
			}
		}
		else if(c == key_enter)
		{
			nextLine();
			if(index > 0)
			{
				if(strcmp(buf, "help") == 0)
				{
					printf("%s\n", "No commands allowed, yet.");
				}
				else if(strcmp(buf, "calculate") == 0)
				{
					initTasks();
					waitForTaskDone();
				}
				else
				{
					printf("Command not found\n");
				}
			}
			printf(command);
			memset(buf, 0, bufSize);
			index = 0;
		}
		else
		{
			if(index < bufSize)
			{
				putchar(c);
				buf[index++] = c;
				buf[index] = 0;
			}
		}
	}
}