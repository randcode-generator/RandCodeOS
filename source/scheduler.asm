extern savedEIP
extern savedESP
extern savedCR3

extern switchThread
extern getCurrentThreadState
extern loadCurrentThread
extern saveCurrentThread

extern interrupt_return

global scheduler
scheduler:
    call getCurrentThreadState
    cmp eax, 1
    je READY_TO_RUNNING
    jmp RUNNING
READY_TO_RUNNING:
    call loadCurrentThread
    mov eax, dword[savedCR3]
    mov cr3, eax

    mov esp, dword[savedESP]

    ;create new stack with eflag, cs, and eip
    pushfd                      ;save eflag
    pop eax                     ;put eflag in eax
    or eax, 0x200               ;enable interrupts
    push eax
    push cs
    push dword[savedEIP]
    jmp interrupt_return
RUNNING:
    pushad
    mov dword[savedESP], esp
    call saveCurrentThread

    call switchThread

    call getCurrentThreadState
    cmp eax, 1
    je READY_TO_RUNNING

    call loadCurrentThread
    mov eax, dword[savedCR3]
    mov cr3, eax
    mov esp, dword[savedESP]
    popad
    jmp interrupt_return