extern void printStr(const char * msg);
extern void setXY(unsigned short xArg, unsigned short yArg);
extern void clrScreen();
const char *msg = "Welcome to RandcodeOS";

int main()
{
	clrScreen();
	printStr(msg);
	setXY(0, 1);
}
