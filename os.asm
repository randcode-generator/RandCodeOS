%define org 0x1000
[ORG org]
starting:
jmp stage2

msg:            db 'Welcome to RandCodeOS', 0

gdt_info:
; null descriptor: 8 bytes of zeros
    dd      0
    dd      0
; GDT code descriptor -- offset is 0x08
    dw      0xFFFF          ; segment limit (0:15)
    dw      0               ; base address (0:15)
    db      0               ; base address (16:23)
    db      10011010b       ; 1001b-1 for present
                            ;      -00 for privilage level
                            ;      -1 for descriptor type (code or data)
                            ; 1010b-segment type (execute/read)
    db      11001111b       ; 1100b-1 for granularity
                            ;      -1 for operation size (0-16 bit, 1-32 bit)
                            ;      -0 reserved bit
                            ;      -0 available for software use
                            ; 1111b segment limit(16:19)
    db      0               ; base address (24:31)
; GDT data descriptor -- offset is 0x10
    dw      0xFFFF          ; segment limit (0:15)
    dw      0               ; base address (0:15)
    db      0               ; base address (16:23)
    db      10010010b       ; 1001b-1 for present
                            ;      -00 for privilage level
                            ;      -1 for descriptor type (code or data)
                            ; 0010b-segment type (read/write)
    db      11001111b       ; 1100b-1 for granularity
                            ;      -1 for operation size (0-16 bit, 1-32 bit)
                            ;      -0 reserved bit
                            ;      -0 available for software use
                            ; 1111b segment limit(16:19)    
    db      0               ; base address (24:31)
gdt_info_end:
gdt_entry:
    dw      gdt_info_end - gdt_info - 1     ; GDT size
    dd      gdt_info                        ; base of GDT

%macro interrupt 1
    dw   %1                 ; offset(0:15)-address of interrupt function (handler)
    dw   8                  ; code segment (0x08)
    db   0                  ; hard coded all zeros
    db   10001110b          ; 1000b
                            ; 1-segment present flag
                            ; 00-privilage level
                            ; 0-hard coded value
                            ; 1110b
                            ; 1-size of gate 1-32 bit, 0-16 bit
                            ; 110-hard coded value for type interrupt gate
    db   0,0                ; offset(16:31)
%endmacro

idt_info:
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt interrupt33-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
    interrupt IRQ_40_47_interrupt_default-starting+org
idt_info_end:

idt_entry:
    dw idt_info_end - idt_info  ; IDT size
    dd idt_info                 ; base of IDT

stage2:
    cli                     ; disable interrupts
    pusha                   ; push all registers into stack
    lgdt    [gdt_entry]     ; load GDT
    popa                    ; Restores all registers from stack

    lidt    [idt_entry]     ; load IDT

    ; Go into protected mode
    ; set cr0 register to 1
    mov     eax, cr0
    mov     eax, 1
    mov     cr0, eax

    ; Jump to stage3
    ; Note: 0x08 is our code segment as specified in our GDT
    jmp     0x08:stage3

bits 32
print:
    push    ebp                 ; save ebp into stack
    mov     ebp, esp            ; copy esp into ebp
    mov     edx, [ebp+8]        ; y 
    mov     ecx, [ebp+12]       ; x
    mov     ebx, [ebp+16]       ; address of string
    sub     esp, 8              ; move stack pointer by 8 (2 local variable)

    mov     eax, 0
    mov     [ebp-4], eax        ; initialize to 0 (string counter)
    mov     [ebp-8], eax        ; initialize to 0 (memory counter)

    ; formula:
    ; desired video memory location = y * 80 * 2 + x
    ; Note: 80 is the number of characters per line.
    ;       Times 2 for character and color.
    mov     eax, 0xb8000        ; start of color video memory
    imul    edx, 160            ; edx is y and 160=80*2
    add     ecx, edx            ; ecx is x
    add     eax, ecx            ; eax will be our desired memory location

    loop:
    ; must be aligned as the following
    ; byte 1: character
    ; byte 2: the color of character
    mov     edx, [ebp-4]        ; copy value of string counter
    movzx   cx, byte[ebx + edx] ; copy character from msg plus offset into cx
                                ; NOTE: must specify to copy a byte
    mov     edx, [ebp-8]        ; copy value of memory counter
    mov     [eax+edx], cx       ; copy character (cx) into memory
    mov     byte[eax+edx+1], 5  ; 5 is the color Magenta
    add     byte[ebp-4], 1      ; add 1 to string counter
    add     byte[ebp-8], 2      ; add 2 to memory counter 
    test    cx, cx              ; test for 0
    jnz     loop                ; if not zero, go to loop

    mov esp, ebp                ; copy ebp to esp. All local variables are popped
    pop ebp                     ; copy the top of the stack to ebp, restore the original ebp value
    ret

interrupt_default:
iret

IRQ_32_39_interrupt_default:
    mov al, 0x20        ; EOI command
    out 0x20, al        ; send to master
iret

IRQ_40_47_interrupt_default:
    mov al, 0x20        ; EOI command
    out 0x20, al        ; send to master
    out 0xA0, al        ; send to slave
iret

VIDEO_MEMEORY:
    dd 0xB80A0          ; 0xB8000 + 80 * 2
                        ; 80 characters per line
                        ; 2 bytes per character

keyscan:
    times 0x90 db 0
    db 'q','w','e','r','t','y','u','i','o','p'
    times 4 db 0
    db 'a','s','d','f','g','h','j','k','l'
    times 5 db 0
    db 'z','x','c','v','b','n','m'
    times 6 db 0
    db ' '

interrupt33:

poll_status:
    ; bit 0 is the output buffer status
    ; 1 means buffer is full, can be read
    ; 0 means buffer is empty, don't read
    mov eax, 0              ; set eax to 0
    in al, 0x64             ; read status from keyboard
    and al, 1               ; zero out all bits except first
    cmp al, 1               ; compare bits 
    jne poll_status         ; jump if not equal

    in al, 0x60             ; read from buffer

    mov cx, ax
    cmp cx, 0x80            ; compare cx to 0x80
    jl cont                 ; if cx is less then 0x80 go to cont

    mov ebx, [VIDEO_MEMEORY]    ; copy video memory to ebx
    and eax, 0x00FF             ; zero all bytes except the first
    mov cl, [keyscan+eax]       ; map the keyscan to ascii
    mov byte[ebx], cl           ; copy to video memory
    mov cl, 15                  ; set the character color to white
    mov byte[ebx+1], cl         ; copy to second byte of video memory
    add word[VIDEO_MEMEORY], 2  ; add 2 to video memory variable

cont:
    mov al, 0x20                ; EOI command
    out 0x20, al                ; send EOI command to master
iret

stage3:
    mov     eax, 0x10
    mov     ds, eax     ; set ds to 0x10 (as specified in GDT)

    lea     eax, [msg]  ; load address of msg to eax
    push    eax         ; push into stack (this will be parameter 1)
    push    0           ; x
    push    0           ; y
    call    print
    add     esp, 12     ; pop off the stack

    ; remap the IRQ by sending
    ; commands to the PIC
    ; http://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interrupt_Controller#Remapping
    mov al, 0x11
    out 0x20, al
    out 0xA0, al
    mov al, 0x20
    out 0x21, al
    mov al, 0x28
    out 0xA1, al
    mov al, 0x04
    out 0x21, al
    mov al, 0x02
    out 0xA1, al
    mov al, 0x01
    out 0x21, al
    out 0xA1, al
    mov al, 0x0
    out 0x21, al
    out 0xA1, al

    sti                 ; enable interrupt

    jmp $
times 1024-($-$$) db 0
