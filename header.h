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
	struct instruction *inst;
	int size;
	int sp, bp, pc;
	int *array;
} stack;

// function prototypes
stack *alloc();

void print_stack(stack *s);

void free_stack(stack *s);

char *check_op(int op_code);


stack *push(stack *s, int n);

stack *pop(stack *s);

stack *push_at_address(stack *s);

stack *store_n_pop(stack *s, int o);

stack *init_stack(stack *s, int m);

stack *jump_cond(stack *s, int a);

stack *out_and_pop(stack *s);

stack *store_char(stack *s, int n);


#endif
