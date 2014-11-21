extern savedEIP
extern savedESP
extern savedCR3

extern switchTask
extern getState
extern loadCurrentThread
extern saveCurrentThread

global scheduler
scheduler:
    call getState
    cmp eax, 1
    je READY_TO_RUNNING
    jmp SAVE_STATE

READY_TO_RUNNING:
    call loadCurrentThread
    mov eax, dword[savedCR3]
    mov cr3, eax

    mov esp, dword[savedESP]
    jmp [savedEIP]

SAVE_STATE:
    pushad
    mov dword[savedESP], esp
    call saveCurrentThread

    call switchTask

    call getState
    cmp eax, 1
    je READY_TO_RUNNING

RUNNING_STATE:
    call loadCurrentThread
    mov eax, dword[savedCR3]
    mov cr3, eax
    mov esp, dword[savedESP]
    popad
    jmp [savedEIP]