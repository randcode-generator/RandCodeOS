[ORG 0x7c00]
  
jmp stage2

msg: db 'Welcome to RandCodeOS', 0

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

stage2:
    cli                     ; disable interrupts
    pusha                   ; push all registers into stack
    lgdt    [gdt_entry]     ; load GDT
    popa                    ; Restores all registers from stack

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

stage3:
    mov     eax, 0x10
    mov     ds, eax     ; set ds to 0x10 (as specified in GDT)

    lea     eax, [msg]  ; load address of msg to eax
    push    eax         ; push into stack (this will be parameter 1)
    push    0           ; x
    push    0           ; y
    call    print
    add     esp, 12     ; pop off the stack

    hlt

    times 510-($-$$) db 0
    db 0x55
    db 0xAA