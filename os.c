extern void printStr(const char * msg);
extern void setXY(unsigned short xArg, unsigned short yArg);
const char *msg = "Welcome to RandcodeOS";

int main()
{
	printStr(msg);
	setXY(10, 24);
	printStr(msg);
}