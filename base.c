// base file for stack machine

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512

// macro i use to toggle between if i need to debug stuff
#define DEBUG 1

// prints stack elements
void print_stack(stack *bp)
{
	int curr;
	int i;

	if (bp == NULL)
	{
		printf("NULL stack :(\n");
		return;
	}

	curr = bp->sp-1;

	// print stack from top to bottom
	while (curr >= 0)
	{
		printf("%d\n", bp->array[curr]);
		curr--;
	}
}

// 1 (LIT) push an element onto the stack, returns ptr to changed stack. assumes stack is not NULL
stack *push(stack *bp, int n)
{
	bp->array[bp->sp] = n;

	// update size and sp before returning
	bp->sp++;
	bp->size++;

	return bp;
}

// 2 (RTN)

// 3 (CAL)

// 4 (POP)

// 5 (PSI)

// 6 (PRM)

// 7 (STO)

// 8 (INC) create a stack with user input, m
stack *create_stack(int m)
{
	stack *bp = malloc(sizeof(stack));
	
	bp->array = calloc(m, sizeof(int));

	bp->sp = 0;
	bp->size = 0;

	return bp;
}

// 9 (JMP)

// 10 (JPC)

// 11 (CHO)

// 12 (CHI)

// 13 (HLT)

// 14 (NBD)

// driver: take in cmd line args containing instructions 
int main(int argc, char *argv)
{
	stack *bp = create_stack(3);
	
	bp = push(bp, 5);
	bp = push(bp, 45646);

	if (DEBUG)
		print_stack(bp);

}
