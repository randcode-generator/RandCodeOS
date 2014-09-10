%define org 0x7c00
[ORG org]

bits 16
Reset:
    mov     ah, 0x0         ; 0x0 means to reset disk
    mov     dl, 0           ; drive 0 is floppy drive
    int     0x13            ; call BIOS
    jc      Reset           ; If Carry Flag (CF) is set, error occurred.
                            ; Try resetting again
 
    ; set es:bx to 0x0:0x1000
    ; es:bx is where the data will be read into
    mov     ax, 0x0         
    mov     es, ax
    mov     bx, 0x1000
 
Read:
    mov     ah, 0x02        ; 0x02 means to read from disk
    mov     al, 1           ; read 1 sector at a time
    mov     ch, 0           ; sector 2 is still on track 1
    mov     cl, 2           ; sector to read (second sector)
    mov     dh, 0           ; head number
    mov     dl, 0           ; drive 0 is floppy drive
    int     0x13            ; read the sector
    jc      Read            ; CF flag is set, error occurred.
                            ; Try to read again

    jmp     0:0x1000        ; jump to execute the sector!

    times 510-($-$$) db 0
    db 0x55
    db 0xAA