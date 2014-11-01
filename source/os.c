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
