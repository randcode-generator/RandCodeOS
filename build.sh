mkdir -p obj

#Compile assembly
nasm -f bin -o obj/boot.img source/boot.asm
nasm -f elf -o obj/os_a.o source/os.asm
nasm -f elf -o obj/dummy_end_section.o source/dummy_end_section.asm

#Compile C code
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/os.c -o obj/os.o 
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/keyboard.c -o obj/keyboard.o 
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/system.c -o obj/system.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/stdio.c -o obj/stdio.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/commandline.c -o obj/commandline.o 
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/string.c -o obj/string.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/memory.c -o obj/memory.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/timer.c -o obj/timer.o


#Link them together
ld -Tlinker.ld -o obj/os.img obj/os_a.o obj/os.o obj/keyboard.o obj/system.o obj/stdio.o obj/commandline.o obj/string.o obj/memory.o obj/timer.o obj/dummy_end_section.o

#Run disassembler (optional)
ndisasm -b16 obj/os.img > nasm.txt

#Output boot.img and os.img into kernel.img
cat obj/boot.img obj/os.img > kernel.img

#Run Qemu with floppy disk image kernel.img
qemu-system-i386 -fda kernel.img