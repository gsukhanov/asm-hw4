section .data  
    const1 dq -6.0
global func1, func2, func3
section .text

;func1(x) = x^sinx
func1:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fsin
    fld qword[ebp + 8]
    fyl2x
    fld st0
    frndint
    fxch
    fsub st0, st1
    f2xm1
    fld1
    faddp
    fscale
    mov esp, ebp
    pop ebp
    ret

;func2(x) = cosx
func2:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fcos
    mov esp, ebp
    pop ebp
    ret

;func3(x) = x - 6
func3:
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fld qword[const1]
    faddp
    mov esp, ebp
    pop ebp
    ret

section .note.GNU-stack