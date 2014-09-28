nasm -f bin -o boot.img boot.asm
nasm -f elf -o os_a.o os.asm
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c os.c -o os.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c keyboard.c -o keyboard.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c system.c -o system.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c stdio.c -o stdio.o

ld -Tlinker.ld -o os.img os_a.o os.o keyboard.o system.o stdio.o
ndisasm -b16 os.img > nasm.txt
cat boot.img os.img > kernel.img
qemu-system-i386 -fda kernel.img
