// auxiliary file for base.c, has all function definitions

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512

// macro i use to toggle between if i need to debug stuff
#define DEBUG 1

// used to denote out of bounds
#define OUT_OF_BOUNDS INT_MIN

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

	s->array = calloc(MAX_STACK_HEIGHT, sizeof(int));

	s->sp = 0;
	s->size = 0;
	s->bp = 0;
	s->pc = 0;
	s->flag = 0;
	s->stop = 0;

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

	return "ERROR";
}

stack *call(stack *s, int op, int field)
{
	switch (op)
	{
		case 1:
			return push(s, field);
		case 2:
			return from_subroutine(s);
		case 3:
			return shift_indices(s, field);
		case 4:
			return pop(s);
		case 5:
			return push_at_address(s);
		case 6:
			return parameter(s, field);
		case 7:
			return store_n_pop(s, field);
		case 8:
			return init_stack(s, field);
		case 9:
			return jump_to_address(s);
		case 10:
			return jump_cond(s, field);
		case 11:
			return out_and_pop(s);
		case 12:
			return s;
		case 13:
			return halt(s);
		case 14:
			return stop_print(s);
		case 15:
			return negate(s);
		case 16:
			return add(s);
		case 17:
			return subtract(s);
		case 18:
			return multiply(s);
		case 19:
			return divide(s);
		case 20:
			return modulo(s);
		case 21:
			return equals(s);
		case 22:
			return not_equals(s);
		case 23:
			return hard_less(s);
		case 24:
			return less_equal(s);
		case 25:
			return hard_greater(s);
		case 26:
			return greater_equal(s);
		case 27:
			return self_push(s);
		default:
			return s;
	}
}

// instructions

// 1 (LIT) hard push an element onto the stack, returns ptr to changed stack. 
// assumes stack is not NULL
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

// 2 (RTN) return from a subroutine
stack *from_subroutine(stack *s)
{
	if (s->sp-1 < 0 || s->sp-2 < 0)
	{
		printf("Trying to pop an empty stack!\n");

		s->stop = -1;
		// s->flag = -1;
		s->inst->op = 2;
		
		return s;
	}

	s->pc = s->array[s->sp-1];
	s->bp = s->array[s->sp-2];
	s->sp -= 2;

	s->inst->op = 2;

	return s;
}

// 3 (CAL) shift focus to index p, making some changes to the stack along the way
stack *shift_indices(stack *s, int p)
{
	s->pc++;
	s->array[s->sp] = s->bp;
	s->array[s->sp+1] = s->pc;

	s->bp = s->sp;
	s->sp += 2;
	s->pc = p;

	s->inst->op = 3;

	return s;
}

// 4 (POP) hard pops top element from the stack
stack *pop(stack *s)
{
	if (s->sp-1 < 0 || s->sp-2 < 0)
	{
		printf("Trying to pop an empty stack!");

		// s->flag = -1;
		s->stop = -1;
		s->inst->op = 4;
		
		return s;
	}

	s->sp--;
	s->size--;

	// update pc
	s->pc++;

	s->inst->op = 4;

	return s;
}

// 5 (PSI) go to the address of the value at stack[sp-1] and push it
stack *push_at_address(stack *s)
{
	// updates pc
	s->array[s->sp-1] = s->array[s->array[s->sp-1]];
	s->pc++;

	s->inst->op = 5;

	return s;
}
// 6 (PRM) Parameter at stack[BP − o] is pushed on the stack
stack *parameter(stack *s, int o)
{
	s->array[s->sp] = s->array[s->bp - o];

	s->sp++;
	s->pc++;

	s->inst->op = 6;
	s->inst->m = o;

	return s;
}

// 7 (STO) Store stack[SP − 2] into the stack at address stack[SP − 1] + o and soft pop
// the stack twice
stack *store_n_pop(stack *s, int o)
{
	if (s->sp-1 < 0 || s->sp-2 < 0)
	{
		printf("Trying to pop an empty stack!\n");
		
		// s->flag = -1;
		s->stop = -1;

		return s;
	}

	int second_to_last = s->array[s->sp-2];
	s->array[s->sp-1 + o] = second_to_last;

	s->sp -= 2;

	// updates pc
	s->pc++;

	s->inst->op = 7;

	return s;
}

// 8 (INC) soft push m amount of 0s ot the top of the stack
stack *init_stack(stack *s, int m)
{	
	s->sp += m;
	s->size += m;

	s->pc++;

	s->inst->op = 8;

	return s;
}

// 9 (JMP) Jump to the address in stack[SP − 1] and pop
stack *jump_to_address(stack *s)
{
	s->pc = s->array[s->sp-1];
	s->sp--;

	return s;
}

// 10 (JPC) if sp-1 is not 0, jump to a specified address, a, updating pc accordingly
// and also popping the stack.
stack *jump_cond(stack *s, int a)
{
	if (s->array[s->sp-1] != 0)
	{
		s->pc = a;
		s->inst->op = 10;

		s->sp--;
		s->size--;
		s->inst->m = a;

		s->inst->op = 9;

		return s;
	}

	else
	{
		s->inst->op = 10;
		s->pc++;
		s->sp--;
		return s;
	}
}

// 11 (CHO) output and hard pop val at top of stack
stack *out_and_pop(stack *s)
{
	if (s->sp-1 < 0 || s->sp-2 < 0)
	{
		printf("Trying to pop an empty stack!\n");
		
		// s->flag = -1;
		s->stop = -1;

		return s;
	}

	s = pop(s);
	int temp = s->array[s->sp];
	char out = (char) temp;

	// pop takes care of updating pc, as well as popping from the stack
	printf("%c", out);

	s->inst->op = 11;

	return s;
}

// 12 (CHI) read in character value and push as an int
stack *store_char(stack *s, char in)
{
	int n = (int) (in - 0);

	s = push(s, n);

	return s;
}

void test(stack *s)
{
	s = push(s, 1);
	s = push(s, 2);
	s = push(s, 3);

	print_stack(s);

	printf("\n");

	s = store_char(s, 'Y');

	print_stack(s);
}

// 13 (HLT)
stack *halt(stack *s)
{
	s->flag = -1;
	s->pc++;
	return s;
}

// 14 (NBD)
stack *stop_print(stack *s)
{
	s->flag = 1;
	s->stop = 1;
	s->pc++;
	return s;
}


// 15 (NEG)
stack *negate(stack *s)
{
	// SHOULD ADD EDGE CASE IF SP < 1

	s->array[s->sp - 1] = s->array[s->sp - 1] * -1;
	s->pc++;
	return s;
}

// 16 (ADD)
stack *add(stack *s)
{
	// SHOULD ADD EDGE CASE IF SP < 2 (functions 16 - 26)

	//	stack[SP - 2]	=	stack[SP - 1]     +	  stack[SP - 2]
	s->array[s->sp - 2] = s->array[s->sp - 1] + s->array[s->sp - 2];
	s->sp--;
	s->pc++;
	return s; 
}
// 17 (SUB)
stack *subtract(stack *s)
{
	//	stack[SP - 2]	=	stack[SP - 1]     -	  stack[SP - 2]
	s->array[s->sp - 2] = s->array[s->sp - 1] - s->array[s->sp - 2];
	s->sp--;
	s->pc++;
	return s;
}

// 18 (MUL)
stack *multiply(stack *s)
{
	//	stack[SP - 2]	=	stack[SP - 1]     *	  stack[SP - 2]
	s->array[s->sp - 2] = s->array[s->sp - 1] * s->array[s->sp - 2];
	s->sp--;
	s->pc++;
	return s;
}

// 19 (DIV)
stack *divide(stack *s)
{
	if (s->array[s->sp - 2] == 0)
	{
          printf("Divisor is zero in DIV instruction!\n");

          // put your new flag that halts the program here
		  s->stop = -1;

          return s;
	}

	//	stack[SP - 2]	=	stack[SP - 1]     /	  stack[SP - 2]
	s->array[s->sp - 2] = s->array[s->sp - 1] / s->array[s->sp - 2];
	s->sp--;
	s->pc++;
	return s;
}

// 20 (MOD)
stack *modulo(stack *s)
{
	if (s->array[s->sp - 2] == 0)
	{
          printf("Modulus is zero in MOD instruction!\n");

          // put your new flag that halts the program here
		  s->stop = -1;

          return s;
	}


	//	stack[SP - 2]	=	stack[SP - 1]     %	  stack[SP - 2]
	s->array[s->sp - 2] = s->array[s->sp - 1] % s->array[s->sp - 2];
	s->sp--;
	s->pc++;
	return s;

}

// 21 (EQL)
stack *equals(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] == s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// 22 (NEQ)
stack *not_equals(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] != s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// 23 (LSS)
stack *hard_less(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] < s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// 24 (LEQ)
stack *less_equal(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] <= s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// 25 (GTR)
stack *hard_greater(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] > s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// 26 (GEQ)
stack *greater_equal(stack *s)
{
	s->array[s->sp - 2] = (s->array[s->sp - 1] >= s->array[s->sp - 2]);
	s->sp--;
	s->pc++;
	return s;
}

// PSP
stack *self_push(stack *s)
{
	s->array[s->sp] = s->sp;
	s->sp++;
	s->pc++;
	return s;
}

