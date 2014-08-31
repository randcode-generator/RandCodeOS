[ORG 0x7c00]
  
jmp main

main:
    hlt
      
    times 510-($-$$) db 0
    db 0x55
    db 0xAA