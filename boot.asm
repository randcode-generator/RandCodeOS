[ORG 0x7c00]
  
jmp main

msg db 'Welcome to RandCodeOS', 0

; reference
; http://en.wikipedia.org/wiki/INT_10H
main:
    mov     ah, 0xe         ; 0xe is used for teletype output
    mov     edx, 0          ; edx is our string offset
    ch_loop:
    mov     al, [msg + edx] ; copy character into al from string offset
    int     0x10            ; call interrupt 0x10 to output character
    inc     edx             ; increment edx (move to next character)
    test    al, al          ; test for 0
    jnz     ch_loop         ; if not 0, go to ch_loop

    hlt

    times 510-($-$$) db 0
    db 0x55
    db 0xAA