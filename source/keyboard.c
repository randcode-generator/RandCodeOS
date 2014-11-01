unsigned char keyPress = 0;
unsigned char keyscan[] =
{
	0,0,'1','2','3','4','5','6','7','8','9','0',
	'-','=',0x8,0,
	'q','w','e','r','t','y','u','i','o','p',
	'[',']',0xD,0,
	'a','s','d','f','g','h','j','k','l',
	';','\'','`',0,'\\',
	'z','x','c','v','b','n','m',
	',','.','/',0,0,0,
	' '
};

unsigned char shiftkeyscan[] =
{
	0,0,'!','@','#','$','%','^','&','*','(',')',
	'_','+',0,0,
	'Q','W','E','R','T','Y','U','I','O','P',
	'{','}',0,0,
	'A','S','D','F','G','H','J','K','L',
	':','"','~',0,'|',
	'Z','X','C','V','B','N','M',
	'<','>','?',0,0,0,
	0
};

void keyboard()
{
	static unsigned char isShift = 0;
	unsigned char rv = 0;
	while(rv & 1 != 1)
		rv = inportb(0x64);

	rv = inportb(0x60);
	rv = rv & 0xFF;

	if(rv==0x2A||rv==0x36)
	{
		isShift=1;
		return;
	}
	else if(rv==0xAA||rv==0xB6)
	{
		isShift=0;
		return;
	}
	
	if(rv<0x80 && isShift==0)
	{
		unsigned char c = keyscan[rv];
		if(c != 0)
			keyPress = c;
	}
	else if(rv<0x80 && isShift==1)
	{
		unsigned char c = shiftkeyscan[rv];
		if(c != 0)
			keyPress = c;
	}
}