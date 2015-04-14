mkdir -p obj

#Compile assembly
nasm -f bin -o obj/boot.img source/boot.asm
nasm -f elf -o obj/os_a.o source/os.asm
nasm -f elf -o obj/scheduler_a.o source/scheduler.asm
nasm -f elf -o obj/dummy_end_section.o source/dummy_end_section.asm

#Compile C code
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/os.c -o obj/os.o 
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/keyboard.c -o obj/keyboard.o 
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/system.c -o obj/system.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/stdio.c -o obj/stdio.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/string.c -o obj/string.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/memory.c -o obj/memory.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/timer.c -o obj/timer.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/scheduler.c -o obj/scheduler.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/thread.c -o obj/thread.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/process.c -o obj/process.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/threadQueue.c -o obj/threadQueue.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/tasks.c -o obj/tasks.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/commandline.c -o obj/commandline.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -Iinclude -c source/PMM.c -o obj/PMM.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/A20.c -o obj/A20.o
gcc -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -c source/VMM.c -o obj/VMM.o

#Link them together
ld -Tlinker.ld -o obj/os.img obj/os_a.o obj/os.o obj/keyboard.o obj/system.o obj/stdio.o obj/string.o obj/memory.o obj/timer.o obj/thread.o obj/scheduler.o obj/process.o obj/threadQueue.o obj/tasks.o obj/commandline.o obj/PMM.o obj/scheduler_a.o obj/A20.o obj/VMM.o obj/dummy_end_section.o

#Run disassembler (optional)
ndisasm -b32 obj/os.img > nasm.txt

#Output boot.img and os.img into kernel.img
cat obj/boot.img obj/os.img > kernel.img

#Run Qemu with floppy disk image kernel.img
qemu-system-i386 -fda kernel.img