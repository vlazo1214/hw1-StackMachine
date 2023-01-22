// base file for stack machine

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512

// macro i use to toggle between if i need to debug stuff
#define DEBUG 1

// aux functions

// print stack from top to bottom
void print_stack(stack *s)
{
	int curr;

	if (s == NULL)
	{
		printf("NULL stack :(\n");
		return;
	}

	curr = s->sp-1;

	while (curr >= 0)
	{
		printf("%d\n", s->array[curr]);
		curr--;
	}
}

// allocates space for stack fields except array (done in INC).
// did this to be able to print bp, sp, and pc fields when program starts
stack *alloc()
{
	stack *s = malloc(sizeof(stack));

	s->sp = 0;
	s->size = 0;
	s->bp = 0;
	s->pc = 0;

	return s;
}

// instructions

// 1 (LIT) push an element onto the stack, returns ptr to changed stack. assumes stack is not NULL
stack *push(stack *s, int n)
{
	s->array[s->sp] = n;

	// update size, sp, and pc before returning
	s->sp++;
	s->size++;
	s->pc++;

	return s;
}

// 2 (RTN)

// 3 (CAL)

// 4 (POP) pops top element from the stack
int pop(stack *s)
{
	int ret = s->array[s->sp-1];

	// delete element by overridding value to 0
	s->array[s->sp-1] = 0;

	s->sp--;
	s->size--;

	s->pc++;

	return ret;
}

// 5 (PSI)

// 6 (PRM)

// 7 (STO)

// 8 (INC) init stack with user input, m
stack *init_stack(stack *s, int m)
{	
	s->array = calloc(m, sizeof(int));

	s->pc++;

	return s;
}

// 9 (JMP)

// 10 (JPC)

// 11 (CHO) output and pop val at top of stack
stack *out_and_pop(stack *s)
{
	// pop takes care of updating pc, as well as popping from the stack
	printf("%d\n", pop(s));

	return s;
}

// 12 (CHI) read in character value and push as an int
stack *store_char(stack *s, int n)
{
	s = push(s, atoi(input));

	return s;
}

// 13 (HLT)

// 14 (NBD)

// driver: take in cmd line args containing instructions 
int main(int argc, char *argv)
{
	stack *s = alloc();
	s = init_stack(s, 2);
	
	s = push(s, 5);
	s = push(s, 45646);

	if (DEBUG)
	{
		printf("initial stack:\n");
		printf("pc: %d\n", s->pc);
		print_stack(s);
	}
	
	int n = atoi('a');
	s = store_char(s, n);

	if (DEBUG)
	{
		printf("modified stack:\n");
		printf("pc: %d\n", s->pc);
		print_stack(s);
	}
}
