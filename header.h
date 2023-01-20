// header file for base.c

#ifndef __HEADER_H
#define __HEADER_H

// TODO given struct, need to figure out how to read in input and store in this struct
typedef struct instruction{
	int op; // opcode
	int m; // M
} instruction;

// stack struct: size is the curr number of elements, sp is the next free space (top of stack+1),
// and array is how we're storing the elements
typedef struct stack{
	int size;
	int sp, bp, pc;
	int *array;
} stack;

// function prototypes
stack *alloc();
void print_stack(stack *s);

stack *init_stack(stack *s, int m);
stack *push(stack *s, int n);
int pop(stack *s);

#endif
