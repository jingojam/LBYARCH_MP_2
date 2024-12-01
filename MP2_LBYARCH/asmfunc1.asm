section .data
CONST dd 255.0          ;constant value 255.0 for scaling

section .text
bits 64
default rel
global imgCvtGrayFloatToInt

; parameters passed (guide):

; C function prototype: extern void imgCvtGrayFloatToInt(float** grayScale, int** converted, int rows, int cols);

; in x86-64 assembly:
; rcx -> pointer to the array of pointers (float** grayScale input array)
; rdx -> pointer to the array of pointers (int** converted output array)
; r8  -> number of rows
; r9  -> number of cols

; NOTE!!!!!!!!!!!!!!!!!
; - the assembly/C interfacing works smoothly, and accounts for all inputs/outputs even for larger 100x100 arrays (i tested it), so no need to worry about the interface
; - the outputs however, has slight discrepancies (see issues):
; ISSUES:
; - the program must round floating point values through these conditions:
;   1) if the integer part is even and the fractional part is less than 0.5, then truncate
;   1) if the integer part is even and the fractional part is greater than 0.5, then truncate
;   3) if the integer part is odd and the fractional is less than 0.5, then truncate
;   4) if the integer part is odd and the fractional part is greater than 0.5, then round up
; - rounding issue can be found at label PROCESS_COL
; - "cvtss2si" instruction is a vital part of float->int conversion, but it incorrectly rounds up some values (not all)
; - example of the issue: 114.75 -> 115 (it should result to 114, not 115)

imgCvtGrayFloatToInt:
    xor r10, r10         ; r10 = row index (0)
    movss xmm0, [CONST]  ; load 255.0 into xmm0 (constant multiplier)

PROCESS_ROW:
    cmp r10, r8          ;compare row index with the number of rows
    jge END              ;if processed all rows, jump to END

    ;get address of current row of floats (grayScale[row])
    mov r11, [rcx + r10 * 8] ;load address of the current float row into r11
    ;get address of current row of ints (convert[row])
    mov r13, [rdx + r10 * 8] ;load address of current int row into r13

    xor r12, r12         ;r12 = column index (0 initially)

PROCESS_COL:
    cmp r12, r9          ;compare column index with number of columns
    jge NEXT_ROW         ;if all columns processed, move to next row

    ;access the current float element in the row (grayScale[row][col])
    movss xmm1, [r11 + r12 * 4]  ;load float value at grayScale[row][col] into xmm1
    mulss xmm1, xmm0             ;multiply by 255.0 (constant is in memory loaded in xmm0)

    ;convert float to int (adjust to 0-255 and store as uint8_t)
    cvtss2si eax, xmm1          ;convert result to signed integer and store in eax

    ;adjust the value to range 0-255
    cmp eax, 0                  ; Compare eax with 0
    jl PEG_ZERO              ;if eax < 0, jump to PEG_ZERO
    cmp eax, 255                ;compare eax with 255
    jg PEG_MAX               ;if eax > 255, jump to PEG_MAX

STORE_RESULT:
    ;store the adjusted integer result (as uint8_t) back in the converted array
    mov [r13 + r12 * 4], eax     ;store result in the converted array

    inc r12              ;increment column index
    jmp PROCESS_COL      ;jump back to process next column

PEG_ZERO:
    mov eax, 0                  ;set to 0 if less than 0
    jmp STORE_RESULT

PEG_MAX:
    mov eax, 255                ;set to 255 if greater than 255
    jmp STORE_RESULT

NEXT_ROW:
    inc r10              ;increment row index
    jmp PROCESS_ROW      ;jump back to process next row

END:
    ret