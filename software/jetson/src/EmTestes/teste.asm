section .data
    msg db 'Resultado: ', 0
    bk_ln db 0xA
    resultado dd 0
    numero1 dd 10
    numero2 dd 20
    buffer times 10 db 0
buffer_end:

section .text
    global _start

print_string:
    ; ecx = endereço da string fixa
    ; edx = tamanho da string fixa
    mov ecx, msg
    mov edx, 11           ; tamanho da string "Resultado: "
    mov eax, 4            ; sys_write
    mov ebx, 1            ; stdout
    int 0x80
    ret

print_number:
    ; entrada: eax = número a imprimir
    mov ecx, buffer_end

.convert_loop:
    dec ecx
    xor edx, edx
    mov ebx, 10
    div ebx               ; divide eax por 10, resto em edx
    add dl, '0'
    mov [ecx], dl
    cmp eax, 0
    jne .convert_loop

    mov edx, buffer_end
    sub edx, ecx          ; tamanho do número

    mov eax, 4            ; sys_write
    mov ebx, 1
    int 0x80
    ret

pular_linha:
    mov ecx, bk_ln
    mov edx, 1
    mov eax, 4
    mov ebx, 1
    int 0x80
    ret

_start:
    mov eax, [numero1]
    add eax, [numero2]
    mov [resultado], eax

    call print_string
    call pular_linha
    mov eax, [resultado]
    add eax , 10
    call print_number
    call pular_linha
    mov eax, 1            ; sys_exit
    xor ebx, ebx
    int 0x80
