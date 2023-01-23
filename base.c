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

	s->inst = malloc(sizeof(instruction));

	s->sp = 0;
	s->size = 0;
	s->bp = 0;
	s->pc = 0;

	s->inst->op = 0;
	s->inst->m = 0;

	return s;
}

// frees memory occupied by stack
void free_stack(stack *s)
{
	free(s->inst);
	free(s->array);
	free(s);
}

// returns a string (char array) that corresponds with the specified op code. a bit cheeky that 
// there are no default or break statments, just assumes the input is valid
char *check_op(int op_code)
{
	switch (op_code)
	{
		case 1:
			return "LIT";
		case 2:
			return "RTN";
		case 3:
			return "CAL";
		case 4:
			return "POP";
		case 5:
			return "PSI";
		case 6:
			return "PRM";
		case 7:
			return "STO";
		case 8:
			return "INC";
		case 9:
			return "JMP";
		case 10:
			return "JPC";
		case 11:
			return "CHO";
		case 12:
			return "CHI";
		case 13:
			return "HLT";
		case 14:
			return "NDB";
		case 15:
			return "NEG";
		case 16:
			return "ADD";
		case 17:
			return "SUB";
		case 18:
			return "MUL";
		case 19:
			return "DIV";
		case 20:
			return "MOD";
		case 21:
			return "EQL";
		case 22:
			return "NEQ";
		case 23:
			return "LSS";
		case 24:
			return "LEQ";
		case 25:
			return "GTR";
		case 26:
			return "GEQ";
		case 27:
			return "PSP";
	}
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

	s->inst->op = 1;

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

	s->inst->op = 4;

	return ret;
}

// 5 (PSI) go to the address of the value at stack[sp-1] and push it
stack *push_at_address(stack *s)
{
	s = push(s, s->array[s->sp-1]);

	return s;
}

// 6 (PRM)

// 7 (STO)

// 8 (INC) init stack with user input, m
stack *init_stack(stack *s, int m)
{	
	s->array = calloc(m, sizeof(int));

	s->pc++;

	s->inst->op = 8;

	return s;
}

// 9 (JMP)

// 10 (JPC) if sp-1 is not 0, jump to a specified address, a, updating pc accordingly
// and also popping the stack.
stack *jump_cond(stack *s, int a)
{
	if (s->array[s->sp-1] != 0)
	{
		s->pc = a;
		s->inst->op = 10;
		s->inst->m = a;

		pop(s);

		return s;
	}

	else
		return s;
}

// 11 (CHO) output and pop val at top of stack
stack *out_and_pop(stack *s)
{
	// pop takes care of updating pc, as well as popping from the stack
	printf("%d\n", pop(s));

	s->inst->op = 11;

	return s;
}

// 12 (CHI) read in character value and push as an int
// stack *store_char(stack *s, int n)
// {
// 	s = push(s, atoi(input));

// 	return s;
// }

// 13 (HLT)

// 14 (NBD)

// driver: take in cmd line args containing instructions 
int main(int argc, char **argv)
{
	stack *s = alloc();

	// read in text file and store accordingly

	printf("Addr\tOP\tM\n");

	// print instructions here

	printf("Tracing...\n");

	// begin printing instuction sequence
	while (!= end of instructions)
	{
		// print pc, bp, and sp

		// print stack (prob nest a for loop)
		
		// print address and instuction
	}

	free_stack(s);
	return 0;
}
