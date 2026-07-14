;------------------------------------------------------------------------------
; perimeter.asm - единица компиляции, вбирающая функции вычисления периметра
;------------------------------------------------------------------------------

extern RECTANGLE
extern TRIANGLE

;----------------------------------------------
global binarySearch
binarySearch:; Function begin
        endbr64                                         ; 00B0 _ F3: 0F 1E. FA
        mov     r8d, dword [rsp+8H]                     ; 00B4 _ 44: 8B. 44 24, 08
        cmp     esi, edx                                ; 00B9 _ 39. D6
        jl      ?_004                                   ; 00BB _ 7C, 0A
        jmp     ?_005                                   ; 00BD _ EB, 2A

; Filling space: 1H
; Filler type: NOP
;       db 90H

ALIGN   8
?_003:  lea     esi, [rax+1H]                           ; 00C0 _ 8D. 70, 01
        cmp     edx, esi                                ; 00C3 _ 39. F2
        jle     ?_005                                   ; 00C5 _ 7E, 22
?_004:  lea     ecx, [rdx+rsi]                          ; 00C7 _ 8D. 0C 32
        mov     eax, ecx                                ; 00CA _ 89. C8
        shr     eax, 31                                 ; 00CC _ C1. E8, 1F
        add     eax, ecx                                ; 00CF _ 01. C8
        sar     eax, 1                                  ; 00D1 _ D1. F8
        movsxd  rcx, eax                                ; 00D3 _ 48: 63. C8
        shl     rcx, 5                                  ; 00D6 _ 48: C1. E1, 05
        cmp     dword [rdi+rcx], r8d                    ; 00DA _ 44: 39. 04 0F
        jz      ?_006                                   ; 00DE _ 74, 20
        jl      ?_003                                   ; 00E0 _ 7C, DE
        lea     edx, [rax-1H]                           ; 00E2 _ 8D. 50, FF
        cmp     edx, esi                                ; 00E5 _ 39. F2
        jg      ?_004                                   ; 00E7 _ 7F, DE
?_005:  movsxd  rax, esi                                ; 00E9 _ 48: 63. C6
        shl     rax, 5                                  ; 00EC _ 48: C1. E0, 05
        cmp     dword [rdi+rax], r8d                    ; 00F0 _ 44: 39. 04 07
        setl    al                                      ; 00F4 _ 0F 9C. C0
        movzx   eax, al                                 ; 00F7 _ 0F B6. C0
        add     eax, esi                                ; 00FA _ 01. F0
        ret                                             ; 00FC _ C3

; Filling space: 3H
; Filler type: Multi-byte NOP
;       db 0FH, 1FH, 00H

ALIGN   8
?_006:  add     eax, 1                                  ; 0100 _ 83. C0, 01
        ret                                             ; 0103 _ C3
; binarySearch End of function

global insertionSort
insertionSort:; Function begin
        ;endbr64                                         ; 0110 _ F3: 0F 1E. FA
        push    r15                                     ; 0114 _ 41: 57
        push    r14                                     ; 0116 _ 41: 56
        push    r13                                     ; 0118 _ 41: 55
        push    r12                                     ; 011A _ 41: 54
        push    rbp                                     ; 011C _ 55
        push    rbx                                     ; 011D _ 53
        mov     dword [rsp-0CH], esi                    ; 011E _ 89. 74 24, F4
        cmp     esi, 1                                  ; 0122 _ 83. FE, 01
        jle     ?_016                                   ; 0125 _ 0F 8E, 0000010E
        mov     r10, rdi                                ; 012B _ 49: 89. FA
        xor     r11d, r11d                              ; 012E _ 45: 31. DB
        mov     ebx, 1                                  ; 0131 _ BB, 00000001
; Filling space: 0AH
; Filler type: Multi-byte NOP
;       db 66H, 2EH, 0FH, 1FH, 84H, 00H, 00H, 00H
;       db 00H, 00H

ALIGN   16
?_007:  mov     r9d, dword [r10+20H]                    ; 0140 _ 45: 8B. 4A, 20
        mov     ebp, dword [r10+24H]                    ; 0144 _ 41: 8B. 6A, 24
        mov     edx, r11d                               ; 0148 _ 44: 89. DA
        mov     r12d, dword [r10+28H]                   ; 014B _ 45: 8B. 62, 28
        mov     r13d, dword [r10+2CH]                   ; 014F _ 45: 8B. 6A, 2C
        mov     r14d, dword [r10+30H]                   ; 0153 _ 45: 8B. 72, 30
        mov     r15d, dword [r10+34H]                   ; 0157 _ 45: 8B. 7A, 34
        movsd   xmm0, qword [r10+38H]                   ; 015B _ F2 41: 0F 10. 42, 38
        test    r11d, r11d                              ; 0161 _ 45: 85. DB
        je      ?_017                                   ; 0164 _ 0F 84, 000000DA
        mov     r8d, r11d                               ; 016A _ 45: 89. D8
        xor     esi, esi                                ; 016D _ 31. F6
        jmp     ?_009                                   ; 016F _ EB, 0F

; Filling space: 7H
; Filler type: Multi-byte NOP
;       db 0FH, 1FH, 80H, 00H, 00H, 00H, 00H

ALIGN   8
?_008:  lea     esi, [rax+1H]                           ; 0178 _ 8D. 70, 01
        cmp     esi, r8d                                ; 017B _ 44: 39. C6
        jge     ?_010                                   ; 017E _ 7D, 1E
?_009:  lea     eax, [rsi+r8]                           ; 0180 _ 42: 8D. 04 06
        sar     eax, 1                                  ; 0184 _ D1. F8
        movsxd  rcx, eax                                ; 0186 _ 48: 63. C8
        shl     rcx, 5                                  ; 0189 _ 48: C1. E1, 05
        cmp     dword [rdi+rcx], r9d                    ; 018D _ 44: 39. 0C 0F
        jz      ?_011                                   ; 0191 _ 74, 25
        jl      ?_008                                   ; 0193 _ 7C, E3
        lea     r8d, [rax-1H]                           ; 0195 _ 44: 8D. 40, FF
        cmp     esi, r8d                                ; 0199 _ 44: 39. C6
        jl      ?_009                                   ; 019C _ 7C, E2
?_010:  movsxd  rax, esi                                ; 019E _ 48: 63. C6
        shl     rax, 5                                  ; 01A1 _ 48: C1. E0, 05
        cmp     dword [rdi+rax], r9d                    ; 01A5 _ 44: 39. 0C 07
        setl    al                                      ; 01A9 _ 0F 9C. C0
        movzx   eax, al                                 ; 01AC _ 0F B6. C0
        add     esi, eax                                ; 01AF _ 01. C6
        jmp     ?_012                                   ; 01B1 _ EB, 08

; Filling space: 5H
; Filler type: Multi-byte NOP
;       db 0FH, 1FH, 44H, 00H, 00H

ALIGN   8
?_011:  lea     esi, [rax+1H]                           ; 01B8 _ 8D. 70, 01
?_012:  cmp     r11d, esi                               ; 01BB _ 41: 39. F3
        jl      ?_015                                   ; 01BE _ 7C, 3B
?_013:  mov     rax, r10                                ; 01C0 _ 4C: 89. D0
; Filling space: 5H
; Filler type: Multi-byte NOP
;       db 0FH, 1FH, 44H, 00H, 00H

ALIGN   8
?_014:  movdqu  xmm1, oword [rax]                       ; 01C8 _ F3: 0F 6F. 08
        movdqu  xmm2, oword [rax+10H]                   ; 01CC _ F3: 0F 6F. 50, 10
        sub     edx, 1                                  ; 01D1 _ 83. EA, 01
        sub     rax, 32                                 ; 01D4 _ 48: 83. E8, 20
        movups  oword [rax+40H], xmm1                   ; 01D8 _ 0F 11. 48, 40
        movups  oword [rax+50H], xmm2                   ; 01DC _ 0F 11. 50, 50
        cmp     edx, esi                                ; 01E0 _ 39. F2
        jge     ?_014                                   ; 01E2 _ 7D, E4
        lea     eax, [rbx-1H]                           ; 01E4 _ 8D. 43, FF
        mov     edx, esi                                ; 01E7 _ 89. F2
        sub     edx, eax                                ; 01E9 _ 29. C2
        cmp     esi, ebx                                ; 01EB _ 39. DE
        mov     eax, edx                                ; 01ED _ 89. D0
        mov     edx, 0                                  ; 01EF _ BA, 00000000
        cmovge  eax, edx                                ; 01F4 _ 0F 4D. C2
        lea     edx, [rbx+rax-2H]                       ; 01F7 _ 8D. 54 03, FE
?_015:  movsxd  rax, edx                                ; 01FB _ 48: 63. C2
        add     ebx, 1                                  ; 01FE _ 83. C3, 01
        add     r11d, 1                                 ; 0201 _ 41: 83. C3, 01
        add     r10, 32                                 ; 0205 _ 49: 83. C2, 20
        add     rax, 1                                  ; 0209 _ 48: 83. C0, 01
        shl     rax, 5                                  ; 020D _ 48: C1. E0, 05
        add     rax, rdi                                ; 0211 _ 48: 01. F8
        mov     dword [rax], r9d                        ; 0214 _ 44: 89. 08
        mov     dword [rax+4H], ebp                     ; 0217 _ 89. 68, 04
        mov     dword [rax+8H], r12d                    ; 021A _ 44: 89. 60, 08
        mov     dword [rax+0CH], r13d                   ; 021E _ 44: 89. 68, 0C
        mov     dword [rax+10H], r14d                   ; 0222 _ 44: 89. 70, 10
        mov     dword [rax+14H], r15d                   ; 0226 _ 44: 89. 78, 14
        movsd   qword [rax+18H], xmm0                   ; 022A _ F2: 0F 11. 40, 18
        cmp     dword [rsp-0CH], ebx                    ; 022F _ 39. 5C 24, F4
        jne     ?_007                                   ; 0233 _ 0F 85, FFFFFF07
?_016:  pop     rbx                                     ; 0239 _ 5B
        pop     rbp                                     ; 023A _ 5D
        pop     r12                                     ; 023B _ 41: 5C
        pop     r13                                     ; 023D _ 41: 5D
        pop     r14                                     ; 023F _ 41: 5E
        pop     r15                                     ; 0241 _ 41: 5F
        ret                                             ; 0243 _ C3
; insertionSort End of function

?_017:  ; Local function
        xor     esi, esi                                ; 0244 _ 31. F6
        cmp     dword [rdi], r9d                        ; 0246 _ 44: 39. 0F
        jge     ?_013                                   ; 0249 _ 0F 8D, FFFFFF71
        mov     edx, r11d                               ; 024F _ 44: 89. DA
        jmp     ?_015                                   ; 0252 _ EB, A7

;----------------------------------------------
; Вычисление периметра прямоугольника
;double PerimeterRectangle(void *r) {
;    return 2.0 * (*((int*)r) + *((int*)(r+intSize)));
;}
global PerimeterRectangle
PerimeterRectangle:
section .text
push rbp
mov rbp, rsp

    ; В rdi адрес прямоугольника
    mov eax, [rdi]
    add eax, [rdi+4]
    shl eax, 1
    cvtsi2sd    xmm0, eax

leave
ret

;----------------------------------------------
; double PerimeterTriangle(void *t) {
;    return (double)(*((int*)t) + *((int*)(t+intSize)) + *((int*)(t+2*intSize)));
;}
global PerimeterTriangle
PerimeterTriangle:
section .text
push rbp
mov rbp, rsp

    ; В rdi адрес треугольника
    mov eax, [rdi]
    add eax, [rdi+4]
    add eax, [rdi+8]
    cvtsi2sd    xmm0, eax

leave
ret

;----------------------------------------------
; Вычисление периметра фигуры
;double PerimeterShape(void *s) {
;    int k = *((int*)s);
;    if(k == RECTANGLE) {
;        return PerimeterRectangle(s+intSize);
;    }
;    else if(k == TRIANGLE) {
;        return PerimeterTriangle(s+intSize);
;    }
;    else {
;        return 0.0;
;    }
;}
global PerimeterShape
PerimeterShape:
section .text
push rbp
mov rbp, rsp

    ; В rdi адрес фигуры
    mov eax, [rdi]
    cmp eax, [RECTANGLE]
    je .rectPerimeter
    cmp eax, [TRIANGLE]
    je .trianPerimeter
    xor eax, eax
    cvtsi2sd    xmm0, eax
    jmp     .return
.rectPerimeter:
    ; Вычисление периметра прямоугольника
    add     rdi, 4
    call    PerimeterRectangle
    jmp     .return
.trianPerimeter:
    ; Вычисление периметра треугольника
    add     rdi, 4
    call    PerimeterTriangle
.return:
leave
ret

;----------------------------------------------
;// Вычисление суммы периметров всех фигур в контейнере
;double PerimeterSumContainer(void *c, int len) {
;    double sum = 0.0;
;    void *tmp = c;
;    for(int i = 0; i < len; i++) {
;        sum += PerimeterShape(tmp);
;        tmp = tmp + shapeSize;
;    }
;    return sum;
;}
global PerimeterSumContainer
PerimeterSumContainer:
section .data
    .sum    dq  0.0
section .text
push rbp
mov rbp, rsp

    ; В rdi адрес начала контейнера
    mov rbx, rsi            ; число фигур
    xor rcx, rcx            ; счетчик фигур
    movsd xmm1, [.sum]      ; перенос накопителя суммы в регистр 1
.loop:
    cmp rcx, rbx            ; проверка на окончание цикла
    jge .return             ; Перебрали все фигуры

    mov r10, rdi            ; сохранение начала фигуры
    call PerimeterShape     ; Получение периметра первой фигуры
    addsd xmm1, xmm0        ; накопление суммы
    inc rcx                 ; индекс следующей фигуры
    add r10, 16             ; адрес следующей фигуры
    mov rdi, r10            ; восстановление для передачи параметра
    jmp .loop
.return:
    movsd xmm0, xmm1
leave
ret
