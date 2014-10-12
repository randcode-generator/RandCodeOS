extern void printStr(const char * msg);
extern void setXY(unsigned short xArg, unsigned short yArg);
extern void clrScreen();
extern void commandline();
extern void init_timer(unsigned short freq);
const char *msg = "Welcome to RandcodeOS";

int main()
{
	clrScreen();
	printf(msg);
	setXY(0, 1);
	setup_paging();
	init_timer(100);

	commandline();
}
