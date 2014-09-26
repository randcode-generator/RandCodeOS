extern keyboard

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
    %rep 32
    interrupt interrupt_default-starting+org
    %endrep
	interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt interrupt_33-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    interrupt IRQ_32_39_interrupt_default-starting+org
    %rep 8
    interrupt IRQ_40_47_interrupt_default-starting+org
    %endrep
idt_info_end:

idt_entry:
    dw idt_info_end - idt_info  ; IDT size
    dd idt_info                 ; base of IDT

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

interrupt_33:
    call keyboard
    mov al, 0x20        ; EOI command
    out 0x20, al        ; send to master
iret
