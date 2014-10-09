extern void printStr(const char * msg);
extern void setXY(unsigned short xArg, unsigned short yArg);
extern void clrScreen();
extern void commandline();
const char *msg = "Welcome to RandcodeOS";

int main()
{
	clrScreen();
	printf(msg);
	setXY(0, 1);
	setup_paging();

	unsigned int *mem = (unsigned int *)0x41D000;
	*mem = 1998;
	printf("value: %u\n", *mem);

	mem = (unsigned int *)0x43D000;
	printf("value: %u\n", *mem);

	mem = (unsigned int *)0x82E000;
	*mem = 6789;
	printf("value: %u\n", *mem);

	*mem = 9998;
	printf("value: %u\n", *mem);

	*mem = 11113;
	printf("value: %u\n", *mem);

	commandline();
}
