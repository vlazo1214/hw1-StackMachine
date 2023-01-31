// base file for stack machine

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


#define MAX_STACK_HEIGHT 2048
#define MAX_CODE_LENGTH 512


// driver: take in cmd line args containing instructions 
int main(int argc, char **argv)
{
	FILE * fp;
	char * filename = argv[1];
	// if file name invalid:
	if (filename == NULL)
	{
		stack *s = alloc();
		test(s);
		return 0;
	}
	
	fp = fopen(filename, "r");
	if (fp == NULL)
		return 1;
	
	// int array to store the instructions for output and access
	//int * inst = malloc((MAX_CODE_LENGTH * 2) * sizeof(int)); 
	int inst[512][2]; 
	int row = 0;
	// int num = 0;
	char * String = calloc(5, sizeof(char));
	// int halt = 0;

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
	int curInst = 0;
	// begin printing instuction sequence

	printf("PC: %d BP: %d SP: %d\n", s->pc, s->bp, s->sp);
	printf("stack: \n");


	while (curInst <= row)
	{
		if (s->flag <= 0)
			printf("==> addr: %d\t%s\t%d\n", s->pc, check_op(inst[curInst][0]), inst[curInst][1]);

		s = call(s, inst[curInst][0], inst[curInst][1]);

		// printf("row = %d\n# of cycles = %d\n", row, curInst);
		// print pc, bp, and sp
		if (s->flag <= 0)
			printf("PC: %d BP: %d SP: %d\n", s->pc, s->bp, s->sp);

		if (s->flag <= 0)
			printf("stack: ");
		int temp = s->bp;
		if (s->flag <= 0)
		{	
			while(temp < s->sp)
			{
				printf("S[%d]: %d ", temp, s->array[temp]);
				temp++;
			}
		
			printf("\n");
		}

		// if we need to halt or pop from empty 
		if (s->flag == -1)
			break;

		curInst++;

	}

	free_stack(s);
	fclose(fp);
	return 0;
}
