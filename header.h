// header file for base.c

#ifndef __HEADER_H
#define __HEADER_H

typedef struct instruction{
	int op; // opcode
	int m; // M
} instruction;

typedef struct stack{
	int size;
	int *sp;
	int *array;
} stack;

// function prototypes
stack *create_stack();
stack *push(int n);

#endif
