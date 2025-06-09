#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CONSTRUCT_HEADER
#define CONSTRUCT_HEADER
void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    else {
        printf("Error! Dynamic memory was not allocated!\n");
        exit(-1);
    }
}

int token_compare(char* token) {
    char ops[8][4] = {"+", "-", "*", "/", "sin", "cos", "tan", "ctg"};
    for(int i = 0; i < 8; i++) {
        if (strcmp(token, ops[i]) == 0) return i;
    }
    return 0;
}

typedef struct list {
    char* string;
    struct list* next;
} line;

int main(void) {
    line *data = xmalloc(sizeof(line));
    data->string = xmalloc(15 * sizeof(char));
    sprintf(data->string, "section .data\n");
    data->next = 0;
    line *text = xmalloc(sizeof(line));
    text->string = xmalloc(42 * sizeof(char));
    sprintf(text->string, "\nglobal func1, func2, func3\n\nsection .text\n");
    text->next = 0;
    double l, r;
    scanf("%lf %lf\n", &l, &r);
    FILE* aux = fopen("build/interval.c", "w");
    fprintf(aux, "#ifndef INTERVAL_HEADER\n#define INTERVAL_HEADER\nconst double l = %lf;\nconst double r = %lf;\n#endif", l, r);
    fclose(aux);
    unsigned counter = 1;
    line *olddata = data;
    line *oldtext = text;
    for(unsigned funcnum = 1; funcnum <= 3; funcnum++) {
        char buffer[500];
        line* prologue = xmalloc(sizeof(line));
        prologue->string = xmalloc(49 * sizeof(char));
        prologue->next = 0;
        sprintf(prologue->string, "func%u:\n    push ebp\n    mov ebp, esp\n    finit\n", funcnum);
        text->next = prologue;
        text = text->next;
        fgets(buffer, 500, stdin);
        char *token = strtok(buffer, " \n");
        while(token) {
            if ((*token >= '0' && *token <= '9')) {
                double val = atof(token);
                line* constline = xmalloc(sizeof(line));
                constline->string = xmalloc(100 * sizeof(char));
                constline->next = 0;
                sprintf(constline->string, "    const%u dq %lf\n", counter, val);
                data->next = constline;
                data = data->next;
                line* pushline = xmalloc(sizeof(line));
                pushline->string = xmalloc(50 * sizeof(char));
                pushline->next = 0;
                sprintf(pushline->string, "    fld qword[const%u]\n", counter);
                text->next = pushline;
                text = text->next;
                counter += 1;
            }
            else if (strcmp(token, "pi") == 0) {
                line* pushline = xmalloc(sizeof(line));
                pushline->string = xmalloc(11 * sizeof(char));
                pushline->next = 0;
                sprintf(pushline->string, "    fldpi\n");
                text->next = pushline;
                text = text->next;
            }
            else if (strcmp(token, "e") == 0) {
                line* pushline = xmalloc(sizeof(line));
                pushline->string = xmalloc(42 * sizeof(char));
                pushline->next = 0;
                sprintf(pushline->string, "    fldl2e\n    f2xm1\n    fld1\n     faddp\n");
                text->next = pushline;
                text = text->next;
            }
            else if (strcmp(token, "x") == 0) {
                line* pushline = xmalloc(sizeof(line));
                pushline->string = xmalloc(24 * sizeof(char));
                pushline->next = 0;
                sprintf(pushline->string, "    fld qword[ebp + 8]\n");
                text->next = pushline;
                text = text->next;
            }
            else {
                int cmp = token_compare(token);
                if (cmp) {
                    char strings[8][38] = {
                        "    faddp\n", 
                        "    fsubp\n", 
                        "    fmulp\n", 
                        "    fdivp\n", 
                        "    fsin\n", 
                        "    fcos\n", 
                        "    fptan\n    fmulp\n", 
                        "    ftan\n    fxch\n    fdivp\n"};
                    line* pushline = xmalloc(sizeof(line));
                    pushline->string = xmalloc(38 * sizeof(char));
                    pushline->next = 0;
                    sprintf(pushline->string, "%s", strings[cmp]);
                    text->next = pushline;
                    text = text->next;
                }
                else {
                    printf("Error! Wrong input!\n");
                    exit(-1);
                }
            }
            token = strtok(0, " \n");
        }
        line* epilogue = xmalloc(sizeof(line));
        epilogue->string = xmalloc(38 * sizeof(char));
        epilogue->next = 0;
        sprintf(epilogue->string, "    mov esp, ebp\n    pop ebp\n    ret\n\n");
        text->next = epilogue;
        text = text->next;
    }
    line* gnu_stack = xmalloc(sizeof(line));
    gnu_stack->string = xmalloc(56);
    gnu_stack->next = 0;
    sprintf(gnu_stack->string, "section .note.GNU-stack noalloc noexec nowrite progbits");
    text->next = gnu_stack;
    text = text->next;
    data = olddata;
    text = oldtext;
    FILE* f = fopen("build/extra_functions.asm", "w");
    do {
        fprintf(f, "%s", data->string);
        line* ndata = data->next;
        free(data->string);
        free(data);
        data = ndata;
    } while (data->next);
    fprintf(f, "%s", data->string);
    line* ndata = data->next;
    free(data->string);
    free(data);
    do {
        fprintf(f, "%s", text->string);
        line* ntext = text->next;
        free(text->string);
        free(text);
        text = ntext;
    } while (text->next);
    fprintf(f, "%s", text->string);
    line* ntext = text->next;
    free(text->string);
    free(text);
    fclose(f);
    return 0;
}
#endif