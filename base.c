// base file for stack machine
// start/Spring 22/SystemsSoftware/hw1-StackMachine (for vin)

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512

// create a stack using user input
// * idk if m is supposed to be there or not or if its supposed to be max_stack_height
stack *create_stack(int m)
{
	stack *bp = malloc(sizeof(stack));
	
	bp->array = malloc(sizeof(m));

	bp->sp = NULL;

	return bp;
}

// returns 0 is stack is null, 1 if size is 0, and 2 if neither. may change this to a
// traditional "return 1 if empty 0 otherwise" if this method is deemed unnecessary
int isEmpty(stack *bp)
{
	if (bp == NULL)
		return 0;
	else if (bp->size == 0)
		return 1;
	else
		return 2;
}

// push an element onto the stack
// stack *push(stack *bp, int n)
// {
// 	// if stack is null, create a stack and return it
// 	if (bp == NULL)
// 	{
// 		stack *new_stack = create_stack();
// 		return new_stack;
// 	}


// }

// driver: take in cmd line args containing instructions 
int main(int argc, char *argv)
{
	stack *bp = create_stack(2);

    printf("hello\n");
}
