#Compile assembly
nasm -f bin -o boot.img boot.asm
nasm -f elf -o os_a.o os.asm
nasm -f elf -o dummy_end_section.o dummy_end_section.asm

#Compile C code
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c os.c keyboard.c system.c stdio.c

#Link them together
ld -Tlinker.ld -o os.img os_a.o os.o keyboard.o system.o stdio.o dummy_end_section.o

#Run disassembler (optional)
ndisasm -b16 os.img > nasm.txt

#Output boot.img and os.img into kernel.img
cat boot.img os.img > kernel.img

#Run Qemu with floppy disk image kernel.img
qemu-system-i386 -fda kernel.img
