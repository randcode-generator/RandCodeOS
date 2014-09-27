OUTPUT_FORMAT(binary)
SECTIONS
{
    /*
        org variable 
    */
	.org = 0x00001000;
    
    /*
        desired OS size (make sure it is divisible by 512)
        Note: this DOES NOT include the boot sector.
        This size only contains operating system logic.
    */
	.desiredExecutableSize = 4608;

    /*
        set current to .org
        Note: It means load the program at 0x1000
    */
    . = .org;

    /*
        load all the text data and rodata (read only data)
        back to back
    */
    .text : { * (.text); }
    .data : { * (.data); }
    .bss : { * (.bss); }
    .rodata : { * (.rodata); }

    /*
        programEnd is assigned the current location
        Note: It is the location of the end of the program
    */
    .programEnd = .;

    /*
        The paddingSize is how many bytes to fill (or pad)
    */
    .paddingSize = (.desiredExecutableSize) - (.programEnd);

    /*
        Set the current location to:
        0x1000 + location of programEnd + paddingSize - 1
        Note: Must minus one because dummy_end_section has one byte
    */
    . = .org + (.programEnd) + (.paddingSize) - 1;

    /*
        Finally put the dummy_end_section here
    */
    .dummy_end_section : { * (.dummy_end_section); }
}