RandCodeOS is another operating system based on a whole bunch of tutorials online.

To compile and run
```
nasm -f bin -o boot.img boot.asm
```
Compile boot.asm as binary

```
nasm -f elf -o os_a.o os.asm
```
Compile os.asm as ELF. We need ELF for linking later
```
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c os.c -o os.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c screen.c -o screen.o
```
Compile os.c and screen.c, all these parameters are need to tell gcc not to put any type of information into the binary
```
ld -Tlinker.ld -o os.img os_a.o os.o screen.o
```
Link up os.o and screen.o and output os.img
```
ndisasm -b16 os.img > nasm.txt
```
Disassemble os.img into assembly
```
cat boot.img os.img > kernel.img
```
Combine boot.img and os.img into a new file called kernel.img
```
qemu-system-i386 -fda kernel.img
```
Run emulator with floppy disk. -fda tells the emulator that kernel.img is the floppy disk image

OR  

Just run 
```
sh build.sh
```
