void keyboard_wait_input() {
	//bit 1:
	//0 - input buffer empty (writable)
	//1 - input buffer full (not writable)
	unsigned char c = 0xff;
	do {
		c = inportb(0x64);
	}while((c & 2) != 0);
}

void keyboard_wait_output() {
	//bit 0:
	//0 - output buffer empty (not readable)
	//1 - output buffer full (readable)
	unsigned char c = 0xff;
	do {
		c = inportb(0x64);
	}while((c & 1) == 0);
}

void keyboard_disable() {
	outportb(0x64, 0xAD);
}

void keyboard_enable() {
	outportb(0x64, 0xAE);
}

void keyboard_read_output_command() {
	outportb(0x64, 0xD0);
}

void keyboard_write_output_command() {
	outportb(0x64, 0xD1);
}

unsigned char keyboard_read_output() {
	return inportb(0x60);
}

unsigned char keyboard_write_output(unsigned char a) {
	return outportb(0x60, a);
}

void enable_A20() {
	keyboard_wait_input();
   	keyboard_disable();

    keyboard_wait_input();
    keyboard_read_output_command();

   	keyboard_wait_output();
    unsigned char c = keyboard_read_output();

	keyboard_wait_input();
	keyboard_write_output_command();

	keyboard_write_output(c | 2);
	keyboard_wait_input();

	keyboard_enable();
	keyboard_wait_input();	
}