section .data
    const1 dq -1.0
    const2 dq -2.0
    const3 dq 3.0
    const4 dq 2.0
global func1, func2, func3
section .text

func1: ; func1(x) = e^x + 2
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp + 8]
    fldl2e
    fmulp
    fld st0
    frndint
    fxch
    fsub st0, st1
    f2xm1
    fld1
    faddp
    fscale
    fld qword[const4]
    faddp
    mov esp, ebp
    pop ebp
    ret 
    
func2: ; func2(x) = -1/x
    push ebp
    mov ebp, esp
    finit
    fld qword[const1]
    fld qword[ebp+8]
    fdivp
    mov esp, ebp
    pop ebp
    ret
    
func3: ;func3(x) = -2(x+1)/3
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
    
section .note.GNU-stack