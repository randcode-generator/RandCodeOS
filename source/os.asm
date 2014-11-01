%define org 0x1000
bits 16
extern main
starting:
jmp stage2

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
%include "source/idt.asm"

; remap the IRQ by sending
; commands to the PIC
; http://en.wikibooks.org/wiki/X86_Assembly/Programmable_Interrupt_Controller#Remapping
remapIRQ:
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
ret

stage3:
    mov     eax, 0x10
    mov     ds, eax     ; set ds to 0x10 (as specified in GDT)

    lidt    [idt_entry]     ; load IDT

	call remapIRQ
	
	sti
	
    call main
    
    ; should never get here
    jmp $
