unsigned char timer_set = 0;

void init_timer(unsigned short freq)
{
	unsigned int divisor = 1193180/freq;
	outportb(0x43, 0x36);

    unsigned char l = (unsigned char)(divisor & 0xFF);
    unsigned char h = (unsigned char)((divisor>>8) & 0xFF);

    outportb(0x40, l);
    outportb(0x40, h);
}

void timer()
{
	static unsigned int i = 0;
	if(i % 100 == 0 && timer_set == 1)
		printf("timer %u\n", i);
	i++;
}