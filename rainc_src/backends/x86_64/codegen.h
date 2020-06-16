#ifndef x86_64_CODEGEN_H
#define x86_64_CODEGEN_H

#define STACK_ELEM_SIZE 8

#define PUSH_NUM \
    "\tpush %lu\n"

#define PUSH_LABEL_ADDR \
    "\tpush %s\n"

#define PUSH_RBP_OFFSET \
    "\tlea rcx, [rbp - %lu]\n" \
    "\tpush rcx\n"

#define SECTION_CODE \
    "section .text\n"

#define SECTION_DATA \
    "section .data\n"

#define SECTION_READONLY \
    "section .rodata\n"

#define NEW_LABEL \
    "%s:\n"

#define GLOBAL \
    "global %s\n"

#define EXTERN_REF \
    "extern %s\n"

#define INC \
    "\tinc qword [rsp]\n"

#define DEC \
    "\tdec qword [rsp]\n"

/* For ADD and SUB, pop into rax for the first param, and then use 
    the second param from the stack by dereferencing it */
#define ADD \
    "\tpop rax\n" \
    "\tadd qword [rsp], rax\n"

#define SUB \
    "\tpop rax\n" \
    "\tsub qword [rsp], rax\n"

/* For MUL and DIV, pop into rax and rbx for parameters,
    then pop only rax for the result */
#define MUL \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\tmul rbx\n" \
    "\tpush rax\n"

#define DIV \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\txor rdx, rdx\n" \
    "\tdiv rbx\n" \
    "\tpush rax\n"

#define MOD \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\txor rdx, rdx\n" \
    "\tdiv rbx\n" \
    "\tpush rdx\n"

#define CALL \
    "\tmov r8, %lu\n" /* Argument count */ \
    "\tpush r8\n" \
    "\tcall %s\n" \
    "\tpop r8\n" \
    "\tmov rcx, r8\n" \
    "\trep pop r8\n" /* Clear the parameters */

#define FUNC_BEGIN \
    "\tpush rbp\n" \
    "\tmov rbp, rsp\n"

#define SET_RETURN_VAL \
    "\tpop rax\n"

#define FUNC_RETURN \
    "\tmov rsp, rbp\n" \
    "\tpop rbp\n" \
    "\tret\n"

#define PEEK_8 \
    "\tpop rbx\n" \
    "\txor rax, rax\n" \
    "\tmov al, byte [rbx]" \
    "\tpush rax\n"

#define PEEK_16 \
    "\tpop rbx\n" \
    "\txor rax, rax\n" \
    "\tmov ax, word [rbx]" \
    "\tpush rax\n"

#define PEEK_32 \
    "\tpop rbx\n" \
    "\txor rax, rax\n" \
    "\tmov eax, dword [rbx]" \
    "\tpush rax\n"

#define PEEK_64 \
    "\tpop rbx\n" \
    "\tmov rax, qword [rbx]" \
    "\tpush rax\n"

#define POKE_8 \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\tmov byte [rbx], al\n" \
    "\tpush rax\n"

#define POKE_16 \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\tmov word [rbx], ax\n" \
    "\tpush rax\n"

#define POKE_32 \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\tmov dword [rbx], eax\n" \
    "\tpush rax\n"

#define POKE_64 \
    "\tpop rax\n" \
    "\tpop rbx\n" \
    "\tmov qword [rbx], rax\n" \
    "\tpush rax\n"

/* For IF and WHILE, the condition results will be stored in rdi */
#define IF \
    "\ttest rdi, rdi\n" \
    "\tjz .__dl%lu\n"

#define WHILE \
    "\ttest rdi, rdi\n" \
    "\tjnz .__dl%lu\n"

#define BREAK_LOOP \
    "\tjmp .__dl%lu\n"

/* Temporary register for shifting things around on the stack */
#define PUSH_FROM_TEMP \
    "\tpush r9\n"

#define POP_TO_TEMP \
    "\tpop r9\n"

#define STRING_LITERAL_BEGIN \
    "__literal%lu: db \""

#define STRING_LITERAL_END \
    "\", 0"

#define DISPOSABLE_LABEL \
    ".__dl%lu:\n"

#endif