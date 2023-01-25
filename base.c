// base file for stack machine

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

/*stack *call(stack *s, int op, int field)
{
	switch (op)
	{
		case 1:
			return
		case 2:
			return
		case 3:
			return
		case 4:
			return
		case 5:
			return
		case 6:
			return
		case 7:
			return
		case 8:
			return init_stack(s, field);
		case 9:
			return
		case 10:
			return
		case 11:
			return
		case 12:
			return
		case 13:
			return
		case 14:
			return
		case 15:
			return
		case 16:
			return
		case 17:
			return
		case 18:
			return
		case 19:
			return
		case 20:
			return
		case 21:
			return
		case 22:
			return
		case 23:
			return
		case 24:
			return
		case 25:
			return
		case 26:
			return
		case 27:
			return
	}

}*/


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
	s->pc = s->array[s->sp-1];
	s->bp = s->array[s->sp-2];
	s->sp -= 2;

	s->inst->op = 2;

	return s;
}

// 3 (CAL) shift focus to index p, making some changes to the stack along the way
stack *shift_indices(stack *s, int p)
{
	s->array[s->sp] = s->bp;
	s->array[s->sp+1] = pc;

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
	s = push(s, s->array[s->sp-1]);

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

// void test(stack *s)
// {
// 	s = push(s, 1);
// 	s = push(s, 2);
// 	s = push(s, 3);
// 	s = push(s, 4);
// 	s = push(s, 5);
// 	s = push(s, 89);

// 	print_stack(s);

// 	printf("\n");

// 	s = jump_cond(s, 23);

// 	print_stack(s);
// }

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

		pop(s);
		s->inst->m = a;

		s->inst->op = 9;

		return s;
	}

	else
	{
		s->inst->op = 10;
		return s;
	}
}

// 11 (CHO) output and hard pop val at top of stack
stack *out_and_pop(stack *s)
{
	s = pop(s);
	int temp = s->array[s->sp];
	char out = (char) temp;

	// pop takes care of updating pc, as well as popping from the stack
	printf("%c\n", out);

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
	FILE * fp;
	char * filename = argv[1];
	// if file name invalid:
	if (filename == NULL)
		return 0;
	
	fp = fopen(filename, "r");
	if (fp == NULL)
		return 1;
	
	// int array to store the instructions for output and access
	//int * inst = malloc((MAX_CODE_LENGTH * 2) * sizeof(int)); 
	int inst[512][2]; 
	int row = 0;
	int num = 0;
	char * String = calloc(5, sizeof(char));

	while (!feof(fp))
	{
		fscanf(fp, "%s", String);
		//inst[row][row % 2] = atoi(String);
		inst[row/2][row%2] = atoi(String);
		row++;
	}

	row--;
	row = row / 2;	

	stack *s = alloc();


	printf("Addr\tOP\tM\n");
	for (int i = 0; i < row; i++)
	{	
		printf("%d\t%s\t%d", i, check_op(inst[i][0]), inst[i][1]);
		printf("\n");
	}

	// print instructions here

	printf("Tracing...\n");
	//printf("row = %d\n", row);
	int curInst = 0;
	// begin printing instuction sequence
	while (curInst < row)
	{
		// printf("row = %d\n# of cycles = %d\n", row, curInst);
		// print pc, bp, and sp
		printf("PC: %d BP: %d SP: %d\n", s->pc, s->bp, s->sp);

		if (curInst == 0)
		{
			s = init_stack(s, inst[0][1]);
			//break;
		}

		printf("stack: ");
		int temp = s->bp;
		/*while(temp <= s->sp)
		{
			printf("S[%d]: %d ", temp, s->array[temp]);
			temp++;
		}*/
		printf("\n");

		printf("==> addr: \n");

		curInst++;
	}

	free_stack(s);
	fclose(fp);
	return 0;
}
