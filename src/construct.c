#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *xmalloc(size_t size) { // A malloc package to correctly catch a memory limit and stop.
    void *ptr = malloc(size);
    if (ptr) return ptr;
    else {
        printf("Error! Dynamic memory was not allocated!\n");
        exit(-1);
    }
}

int token_compare(char* token) { // A comparator function to reduce cluttering in the code
    char ops[8][4] = {"+", "-", "*", "/", "sin", "cos", "tan", "ctg"};
    for(int i = 0; i < 8; i++) {
        if (strcmp(token, ops[i]) == 0) return i;
    }
    return -1;
}

typedef struct list { // Constructing two lists of strings to then write them into the .asm file
    char* string;
    struct list* next;
} line;

int main(void) {
    line *data = xmalloc(sizeof(line));
    data->string = xmalloc(15 * sizeof(char));
    sprintf(data->string, "section .data\n"); // first list is the section .data part, where constant values go
    data->next = 0;
    line *text = xmalloc(sizeof(line));
    text->string = xmalloc(44 * sizeof(char));
    sprintf(text->string, "\nglobal func1, func2, func3\n\nsection .text\n"); // second list is globals + section .text, also ends with section .note.GNU-stack
    text->next = 0;
    double l = 0, r = 0;
    char* filename = getenv("SPEC_FILE");
    FILE* input = fopen(filename, "r");
    fscanf(input, "%lf %lf\n", &l, &r);
    FILE* aux = fopen("build/interval.c", "w"); // also a .c file with root search limits is constructed to pass that information to the main build
    fprintf(aux, "#ifndef INTERVAL_HEADER\n#define INTERVAL_HEADER\nconst double _l = %lf;\nconst double _r = %lf;\n#endif", l, r);
    fclose(aux);
    unsigned counter = 1;
    line *olddata = data;
    line *oldtext = text;
    for(unsigned funcnum = 1; funcnum <= 3; funcnum++) { // the process is done 3 times, once for each function
        char buffer[500];
        line* prologue = xmalloc(sizeof(line));
        prologue->string = xmalloc(49 * sizeof(char));
        prologue->next = 0;
        sprintf(prologue->string, "func%u:\n    push ebp\n    mov ebp, esp\n    finit\n", funcnum);
        text->next = prologue;
        text = text->next;
        fgets(buffer, 500, input);
        char *token = strtok(buffer, " \n\r");// a line of text is tokenized and written on the stack in order
        while(token) { // it is possible since functions are given in the reverse polish notation
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
                sprintf(pushline->string, "    fldl2e\n    fmulp\n    fld st0\n    frndint\n    fxch\n    fsub st0, st1\n    f2xm1\n    fld1\n    faddp\n    fscale");
                text->next = pushline; // writing e takes many instructions since f2xm1 only takes [-1, 1] arguments
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
                int cmp = token_compare(token); // here, the comparator function is used
                if (cmp != -1) {
                    char strings[8][38] = { 
                        "    faddp\n", 
                        "    fsubp\n", 
                        "    fmulp\n", 
                        "    fdivp\n", 
                        "    fsin\n", 
                        "    fcos\n", 
                        "    fptan\n    fmulp\n", 
                        "    fptan\n    fxch\n    fdivp\n"};// need to get rid of 1 on top of the stack with tan, and divide 1 by tan in ctg
                    line* pushline = xmalloc(sizeof(line));
                    pushline->string = xmalloc(38 * sizeof(char));
                    pushline->next = 0;
                    sprintf(pushline->string, "%s", strings[cmp]);
                    text->next = pushline;
                    text = text->next;
                }
                else {
                    printf("Error! Wrong input! Unknown symbol: %s\n", token);
                    exit(-1);
                }
            }
            token = strtok(0, " \n\r");
        }
        line* epilogue = xmalloc(sizeof(line));
        epilogue->string = xmalloc(39 * sizeof(char));
        epilogue->next = 0;
        sprintf(epilogue->string, "    mov esp, ebp\n    pop ebp\n    ret\n\n");
        text->next = epilogue;
        text = text->next;
    }
    line* gnu_stack = xmalloc(sizeof(line));
    gnu_stack->string = xmalloc(56);
    gnu_stack->next = 0;
    sprintf(gnu_stack->string, "section .note.GNU-stack");
    text->next = gnu_stack;
    text = text->next;
    data = olddata;
    text = oldtext;
    fclose(input); // after the input is read the writing begins, starting with section .data
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