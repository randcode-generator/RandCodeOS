nasm -f bin -o boot.img boot.asm
nasm -f elf -o os_a.o os.asm
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c os.c -o os.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c screen.c -o screen.o

ld -Tlinker.ld -o os.img os_a.o os.o screen.o
ndisasm -b16 os.img > nasm.txt
cat boot.img os.img > kernel.img
qemu-system-i386 -fda kernel.img