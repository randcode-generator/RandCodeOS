[ORG 0x7c00]
  
jmp main

msg db 'Welcome to RandCodeOS', 0

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

main:
    cli                     ; disable interrupts
    pusha                   ; push all registers into stack
    lgdt    [gdt_entry]     ; load GDT
    popa                    ; Restores all registers from stack

    ; Go into protected mode
    ; set cr0 register to 1
    mov     eax, cr0
    mov     eax, 1
    mov     cr0, eax

    ; Jump to print
    ; Note: 0x08 is our code segment as specified in our GDT
    jmp     0x08:print

bits 32
print:
    mov     eax, 0x10
    mov     ds, eax             ; set ds to 0x10 (as specified in GDT)
    mov     eax, 0xb8000        ; start of color video memory
    mov     ebx, 0              ; bx is going to be our string offset
    mov     edx, 0              ; dx is going to be our memory offset
    loop:
    ; must be aligned as the following
    ; byte 1: character
    ; byte 2: the color of character
    movzx   cx, byte[msg + ebx] ; copy character from msg plus offset into cx
                                ; NOTE: must specify to copy a byte
    mov     [eax + edx], cx     ; copy character (cx) into memory
    push    cx                  ; save the character by pushing to stack
    mov     cx, 5               ; 5 is the color Magenta
    mov     [eax + edx + 1], cx ; copy into memory
    pop     cx                  ; pop value (character) from stack into cx
    inc     bx                  ; increment bx (move to next character)
    add     edx, 2              ; add memory counter by 2  
    test    cx, cx              ; test for 0
    jnz     loop                ; if not zero, go to loop
 
    hlt

    times 510-($-$$) db 0
    db 0x55
    db 0xAA