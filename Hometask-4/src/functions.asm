section .data
    const1 dq -1.0
    const2 dq -2.0
    const3 dq 3.0
global func1, func2, func3, der1, der2, der3
section .text

func1: ;e^x + 3
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fldl2e
    fmulp
    f2xm1
    fld qword[const3]
    faddp
    mov esp, ebp
    pop ebp
    ret
    
der1: ;e^x
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fldl2e
    fmulp
    f2xm1
    fld1
    faddp
    mov esp, ebp
    pop ebp
    ret
    
func2: ;-1/x
    push ebp
    mov ebp, esp
    finit
    fld qword[const1]
    fld qword[ebp+8]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
der2: ;1/x^2
    push ebp
    mov ebp, esp
    finit
    fld1
    fld qword[ebp+8]
    fdivp
    fld qword[ebp+8]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
func3: ;-2(x+1)/3
    push ebp
    mov ebp, esp
    fld1
    fld qword[ebp + 8]
    faddp
    fld qword[const2]
    fmulp
    fld qword[const3]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
der3: ;-2/3
    push ebp
    mov ebp, esp
    fld qword[const2]
    fld qword[const3]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
section .note.GNU-stack noalloc noexec nowrite progbits