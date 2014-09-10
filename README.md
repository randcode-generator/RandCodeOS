RandCodeOS is another operating system based on a whole bunch of tutorials online.

To build:
```
nasm -f bin -o boot.img boot.asm && nasm -f bin -o os.img os.asm
```

To merge boot and os files:
```
cat boot.img os.img > kernel.img
```

To run:
```
qemu-system-i386 -fda kernel.img
```