; Disassembly of file: main.o
; Mon Nov 29 01:27:32 2021
; Type: ELF64
; Syntax: NASM
; Instruction set: SSE2, x64

default rel

global errMessage1: function
global errMessage2: function
global GetDistance: function
global Print: function
global binarySearch: function
global insertionSort: function
global TaskCars: function
global main: function
global TRIANGLE
global RECTANGLE
global maxSize
global shapeSize
global trianSize
global rectSize
global intSize

extern rand                                             ; near
extern malloc                                           ; near
extern __printf_chk                                     ; near
extern puts                                             ; near
extern _GLOBAL_OFFSET_TABLE_                            ; byte


SECTION .text   align=16 execute                        ; section number 1, code

errMessage1:; Function begin
        endbr64                                         ; 0000 _ F3: 0F 1E. FA
        lea     rdi, [rel .LC0]                         ; 0004 _ 48: 8D. 3D, 00000000(rel)
        jmp     puts                                    ; 000B _ E9, 00000000(PLT r)
; errMessage1 End of function

errMessage2:; Function begin
        endbr64                                         ; 0010 _ F3: 0F 1E. FA
        lea     rdi, [rel .LC1]                         ; 0014 _ 48: 8D. 3D, 00000000(rel)
        jmp     puts                                    ; 001B _ E9, 00000000(PLT r)
; errMessage2 End of function

GetDistance:; Function begin
        endbr64                                         ; 0020 _ F3: 0F 1E. FA
        pxor    xmm0, xmm0                              ; 0024 _ 66: 0F EF. C0
        cvtsi2sd xmm0, dword [rdi+14H]                  ; 0028 _ F2: 0F 2A. 47, 14
        divsd   xmm0, qword [rdi+18H]                   ; 002D _ F2: 0F 5E. 47, 18
        cvttsd2si eax, xmm0                             ; 0032 _ F2: 0F 2C. C0
        mov     dword [rdi], eax                        ; 0036 _ 89. 07
        ret                                             ; 0038 _ C3
; GetDistance End of function

; Filling space: 7H
; Filler type: Multi-byte NOP
;       db 0FH, 1FH, 80H, 00H, 00H, 00H, 00H

ALIGN   8

Print:  ; Function begin
        endbr64                                         ; 0040 _ F3: 0F 1E. FA
        test    esi, esi                                ; 0044 _ 85. F6
        jle     ?_002                                   ; 0046 _ 7E, 58
        lea     eax, [rsi-1H]                           ; 0048 _ 8D. 46, FF
        push    r12                                     ; 004B _ 41: 54
        shl     rax, 5                                  ; 004D _ 48: C1. E0, 05
        push    rbp                                     ; 0051 _ 55
        lea     rbp, [rel .LC2]                         ; 0052 _ 48: 8D. 2D, 00000000(rel)
        push    rbx                                     ; 0059 _ 53
        lea     r12, [rdi+rax+20H]                      ; 005A _ 4C: 8D. 64 07, 20
        mov     rbx, rdi                                ; 005F _ 48: 89. FB
; Filling space: 6H
; Filler type: Multi-byte NOP
;       db 66H, 0FH, 1FH, 44H, 00H, 00H

ALIGN   8
?_001:  movsd   xmm0, qword [rbx+18H]                   ; 0068 _ F2: 0F 10. 43, 18
        mov     ecx, dword [rbx+4H]                     ; 006D _ 8B. 4B, 04
        mov     rsi, rbp                                ; 0070 _ 48: 89. EE
        mov     edi, 1                                  ; 0073 _ BF, 00000001
        mov     r8d, dword [rbx+14H]                    ; 0078 _ 44: 8B. 43, 14
        mov     edx, dword [rbx]                        ; 007C _ 8B. 13
        mov     eax, 1                                  ; 007E _ B8, 00000001
        add     rbx, 32                                 ; 0083 _ 48: 83. C3, 20
        call    __printf_chk                            ; 0087 _ E8, 00000000(PLT r)
        cmp     rbx, r12                                ; 008C _ 4C: 39. E3
        jnz     ?_001                                   ; 008F _ 75, D7
        pop     rbx                                     ; 0091 _ 5B
        pop     rbp                                     ; 0092 _ 5D
        pop     r12                                     ; 0093 _ 41: 5C
        ret                                             ; 0095 _ C3

; Filling space: 0AH
; Filler type: Multi-byte NOP
;       db 66H, 2EH, 0FH, 1FH, 84H, 00H, 00H, 00H
;       db 00H, 00H

ALIGN   16
?_002:  ret                                             ; 00A0 _ C3
; Print End of function

; Filling space: 0FH
; Filler type: Multi-byte NOP
;       db 66H, 66H, 2EH, 0FH, 1FH, 84H, 00H, 00H
;       db 00H, 00H, 00H, 0FH, 1FH, 40H, 00H

ALIGN   16

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

; Filling space: 0CH
; Filler type: Multi-byte NOP
;       db 66H, 66H, 2EH, 0FH, 1FH, 84H, 00H, 00H
;       db 00H, 00H, 00H, 90H

ALIGN   16

insertionSort:; Function begin
        endbr64                                         ; 0110 _ F3: 0F 1E. FA
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

; Filling space: 0CH
; Filler type: Multi-byte NOP
;       db 66H, 66H, 2EH, 0FH, 1FH, 84H, 00H, 00H
;       db 00H, 00H, 00H, 90H

ALIGN   16

TaskCars:; Function begin
        endbr64                                         ; 0260 _ F3: 0F 1E. FA
        push    r12                                     ; 0264 _ 41: 54
        mov     edi, 320                                ; 0266 _ BF, 00000140
        push    rbp                                     ; 026B _ 55
        push    rbx                                     ; 026C _ 53
        call    malloc                                  ; 026D _ E8, 00000000(PLT r)
        mov     rbp, rax                                ; 0272 _ 48: 89. C5
        mov     r12, rax                                ; 0275 _ 49: 89. C4
        lea     rbx, [rax+140H]                         ; 0278 _ 48: 8D. 98, 00000140
        nop                                             ; 027F _ 90
?_018:  call    rand                                    ; 0280 _ E8, 00000000(PLT r)
        add     r12, 32                                 ; 0285 _ 49: 83. C4, 20
        movsxd  rdx, eax                                ; 0289 _ 48: 63. D0
        mov     ecx, eax                                ; 028C _ 89. C1
        imul    rdx, rdx, 1431655766                    ; 028E _ 48: 69. D2, 55555556
        sar     ecx, 31                                 ; 0295 _ C1. F9, 1F
        shr     rdx, 32                                 ; 0298 _ 48: C1. EA, 20
        sub     edx, ecx                                ; 029C _ 29. CA
        lea     edx, [rdx+rdx*2]                        ; 029E _ 8D. 14 52
        sub     eax, edx                                ; 02A1 _ 29. D0
        add     eax, 1                                  ; 02A3 _ 83. C0, 01
        mov     dword [r12-1CH], eax                    ; 02A6 _ 41: 89. 44 24, E4
        call    rand                                    ; 02AB _ E8, 00000000(PLT r)
        pxor    xmm0, xmm0                              ; 02B0 _ 66: 0F EF. C0
        movsxd  rdx, eax                                ; 02B4 _ 48: 63. D0
        mov     ecx, eax                                ; 02B7 _ 89. C1
        imul    rdx, rdx, 1717986919                    ; 02B9 _ 48: 69. D2, 66666667
        sar     ecx, 31                                 ; 02C0 _ C1. F9, 1F
        sar     rdx, 34                                 ; 02C3 _ 48: C1. FA, 22
        sub     edx, ecx                                ; 02C7 _ 29. CA
        lea     edx, [rdx+rdx*4]                        ; 02C9 _ 8D. 14 92
        add     edx, edx                                ; 02CC _ 01. D2
        sub     eax, edx                                ; 02CE _ 29. D0
        add     eax, 1                                  ; 02D0 _ 83. C0, 01
        cvtsi2sd xmm0, eax                              ; 02D3 _ F2: 0F 2A. C0
        movsd   qword [r12-8H], xmm0                    ; 02D7 _ F2 41: 0F 11. 44 24, F8
        call    rand                                    ; 02DE _ E8, 00000000(PLT r)
        pxor    xmm0, xmm0                              ; 02E3 _ 66: 0F EF. C0
        movsxd  rdx, eax                                ; 02E7 _ 48: 63. D0
        mov     ecx, eax                                ; 02EA _ 89. C1
        imul    rdx, rdx, 1374389535                    ; 02EC _ 48: 69. D2, 51EB851F
        sar     ecx, 31                                 ; 02F3 _ C1. F9, 1F
        sar     rdx, 37                                 ; 02F6 _ 48: C1. FA, 25
        sub     edx, ecx                                ; 02FA _ 29. CA
        imul    edx, edx, 100                           ; 02FC _ 6B. D2, 64
        sub     eax, edx                                ; 02FF _ 29. D0
        add     eax, 1                                  ; 0301 _ 83. C0, 01
        cvtsi2sd xmm0, eax                              ; 0304 _ F2: 0F 2A. C0
        mov     dword [r12-0CH], eax                    ; 0308 _ 41: 89. 44 24, F4
        divsd   xmm0, qword [r12-8H]                    ; 030D _ F2 41: 0F 5E. 44 24, F8
        cvttsd2si eax, xmm0                             ; 0314 _ F2: 0F 2C. C0
        mov     dword [r12-20H], eax                    ; 0318 _ 41: 89. 44 24, E0
        cmp     r12, rbx                                ; 031D _ 49: 39. DC
        jne     ?_018                                   ; 0320 _ 0F 85, FFFFFF5A
        mov     edx, 32                                 ; 0326 _ BA, 00000020
        lea     rsi, [rel .LC3]                         ; 032B _ 48: 8D. 35, 00000000(rel)
        mov     edi, 1                                  ; 0332 _ BF, 00000001
        xor     eax, eax                                ; 0337 _ 31. C0
        call    __printf_chk                            ; 0339 _ E8, 00000000(PLT r)
        lea     rdi, [rel .LC4]                         ; 033E _ 48: 8D. 3D, 00000000(rel)
        call    puts                                    ; 0345 _ E8, 00000000(PLT r)
        mov     rdi, rbp                                ; 034A _ 48: 89. EF
        mov     esi, 10                                 ; 034D _ BE, 0000000A
        call    Print                                   ; 0352 _ E8, 00000000(PLT r)
        mov     esi, 10                                 ; 0357 _ BE, 0000000A
        mov     rdi, rbp                                ; 035C _ 48: 89. EF
        call    insertionSort                           ; 035F _ E8, 00000000(PLT r)
        lea     rdi, [rel .LC5]                         ; 0364 _ 48: 8D. 3D, 00000000(rel)
        call    puts                                    ; 036B _ E8, 00000000(PLT r)
        pop     rbx                                     ; 0370 _ 5B
        mov     rdi, rbp                                ; 0371 _ 48: 89. EF
        mov     esi, 10                                 ; 0374 _ BE, 0000000A
        pop     rbp                                     ; 0379 _ 5D
        pop     r12                                     ; 037A _ 41: 5C
        jmp     Print                                   ; 037C _ E9, FFFFFCBF
; TaskCars End of function


SECTION .data   align=1 noexecute                       ; section number 2, data


SECTION .bss    align=1 noexecute                       ; section number 3, bss


SECTION .rodata.str1.8 align=8 noexecute                ; section number 4, const

.LC0:                                                   ; byte
        db 69H, 6EH, 63H, 6FH, 72H, 72H, 65H, 63H       ; 0000 _ incorrec
        db 74H, 20H, 63H, 6FH, 6DH, 6DH, 61H, 6EH       ; 0008 _ t comman
        db 64H, 20H, 6CH, 69H, 6EH, 65H, 21H, 0AH       ; 0010 _ d line!.
        db 20H, 20H, 57H, 61H, 69H, 74H, 65H, 64H       ; 0018 _   Waited
        db 3AH, 0AH, 20H, 20H, 20H, 20H, 20H, 63H       ; 0020 _ :.     c
        db 6FH, 6DH, 6DH, 61H, 6EH, 64H, 20H, 2DH       ; 0028 _ ommand -
        db 66H, 20H, 69H, 6EH, 66H, 69H, 6CH, 65H       ; 0030 _ f infile
        db 20H, 6FH, 75H, 74H, 66H, 69H, 6CH, 65H       ; 0038 _  outfile
        db 30H, 31H, 20H, 6FH, 75H, 74H, 66H, 69H       ; 0040 _ 01 outfi
        db 6CH, 65H, 30H, 32H, 0AH, 20H, 20H, 4FH       ; 0048 _ le02.  O
        db 72H, 3AH, 0AH, 20H, 20H, 20H, 20H, 20H       ; 0050 _ r:.     
        db 63H, 6FH, 6DH, 6DH, 61H, 6EH, 64H, 20H       ; 0058 _ command 
        db 2DH, 6EH, 20H, 6EH, 75H, 6DH, 62H, 65H       ; 0060 _ -n numbe
        db 72H, 20H, 6FH, 75H, 74H, 66H, 69H, 6CH       ; 0068 _ r outfil
        db 65H, 30H, 31H, 20H, 6FH, 75H, 74H, 66H       ; 0070 _ e01 outf
        db 69H, 6CH, 65H, 30H, 32H, 00H, 00H, 00H       ; 0078 _ ile02...

.LC1:                                                   ; byte
        db 69H, 6EH, 63H, 6FH, 72H, 72H, 65H, 63H       ; 0080 _ incorrec
        db 74H, 20H, 71H, 75H, 61H, 6CH, 69H, 66H       ; 0088 _ t qualif
        db 69H, 65H, 72H, 20H, 76H, 61H, 6CH, 75H       ; 0090 _ ier valu
        db 65H, 21H, 0AH, 20H, 20H, 57H, 61H, 69H       ; 0098 _ e!.  Wai
        db 74H, 65H, 64H, 3AH, 0AH, 20H, 20H, 20H       ; 00A0 _ ted:.   
        db 20H, 20H, 63H, 6FH, 6DH, 6DH, 61H, 6EH       ; 00A8 _   comman
        db 64H, 20H, 2DH, 66H, 20H, 69H, 6EH, 66H       ; 00B0 _ d -f inf
        db 69H, 6CH, 65H, 20H, 6FH, 75H, 74H, 66H       ; 00B8 _ ile outf
        db 69H, 6CH, 65H, 30H, 31H, 20H, 6FH, 75H       ; 00C0 _ ile01 ou
        db 74H, 66H, 69H, 6CH, 65H, 30H, 32H, 0AH       ; 00C8 _ tfile02.
        db 20H, 20H, 4FH, 72H, 3AH, 0AH, 20H, 20H       ; 00D0 _   Or:.  
        db 20H, 20H, 20H, 63H, 6FH, 6DH, 6DH, 61H       ; 00D8 _    comma
        db 6EH, 64H, 20H, 2DH, 6EH, 20H, 6EH, 75H       ; 00E0 _ nd -n nu
        db 6DH, 62H, 65H, 72H, 20H, 6FH, 75H, 74H       ; 00E8 _ mber out
        db 66H, 69H, 6CH, 65H, 30H, 31H, 20H, 6FH       ; 00F0 _ file01 o
        db 75H, 74H, 66H, 69H, 6CH, 65H, 30H, 32H       ; 00F8 _ utfile02
        db 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H       ; 0100 _ ........

.LC2:                                                   ; byte
        db 68H, 61H, 73H, 68H, 3AH, 25H, 35H, 69H       ; 0108 _ hash:%5i
        db 20H, 74H, 79H, 70H, 65H, 3AH, 25H, 35H       ; 0110 _  type:%5
        db 69H, 20H, 74H, 61H, 6EH, 6BH, 3AH, 25H       ; 0118 _ i tank:%
        db 35H, 69H, 20H, 63H, 6FH, 6EH, 73H, 75H       ; 0120 _ 5i consu
        db 6DH, 70H, 74H, 69H, 6FH, 6EH, 3AH, 25H       ; 0128 _ mption:%
        db 2EH, 33H, 6CH, 66H, 0AH, 00H                 ; 0130 _ .3lf..


SECTION .rodata.str1.1 align=1 noexecute                ; section number 5, const

.LC3:                                                   ; byte
        db 73H, 69H, 7AH, 65H, 6FH, 66H, 20H, 63H       ; 0000 _ sizeof c
        db 61H, 72H, 3AH, 25H, 6CH, 69H, 20H, 62H       ; 0008 _ ar:%li b
        db 79H, 74H, 65H, 73H, 0AH, 00H                 ; 0010 _ ytes..

.LC4:                                                   ; byte
        db 62H, 65H, 66H, 6FH, 72H, 65H, 00H            ; 0016 _ before.

.LC5:                                                   ; byte
        db 61H, 66H, 74H, 65H, 72H, 00H                 ; 001D _ after.


SECTION .text.startup align=16 execute                  ; section number 6, code

main:   ; Function begin
        endbr64                                         ; 0000 _ F3: 0F 1E. FA
        sub     rsp, 8                                  ; 0004 _ 48: 83. EC, 08
        xor     eax, eax                                ; 0008 _ 31. C0
        call    TaskCars                                ; 000A _ E8, 00000000(PLT r)
        xor     eax, eax                                ; 000F _ 31. C0
        add     rsp, 8                                  ; 0011 _ 48: 83. C4, 08
        ret                                             ; 0015 _ C3
; main End of function


SECTION .rodata align=4 noexecute                       ; section number 7, const

TRIANGLE:                                               ; dword
        dd 00000002H                                    ; 0000 _ 2 

RECTANGLE:                                              ; dword
        dd 00000001H                                    ; 0004 _ 1 

maxSize: dd 00027100H                                   ; 0008 _ 160000 

shapeSize:                                              ; dword
        dd 00000010H                                    ; 000C _ 16 

trianSize:                                              ; dword
        dd 0000000CH                                    ; 0010 _ 12 

rectSize: dd 00000008H                                  ; 0014 _ 8 

intSize: dd 00000004H                                   ; 0018 _ 4 


SECTION .note.gnu.property align=8 noexecute            ; section number 8, const

        db 04H, 00H, 00H, 00H, 10H, 00H, 00H, 00H       ; 0000 _ ........
        db 05H, 00H, 00H, 00H, 47H, 4EH, 55H, 00H       ; 0008 _ ....GNU.
        db 02H, 00H, 00H, 0C0H, 04H, 00H, 00H, 00H      ; 0010 _ ........
        db 03H, 00H, 00H, 00H, 00H, 00H, 00H, 00H       ; 0018 _ ........


