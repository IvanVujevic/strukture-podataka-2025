#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct postfix* Position;
typedef struct postfix
{
	double number;
	Position Next;
}postfix;

int Push(Position P, double value);
double Pop(Position first);
int read(Position Head, const char* filePath);
bool isEmpty(Position);

int main()
{
	const char* filePath = "postfix.txt";
	postfix head;
	head.number = 0;
	head.Next = NULL;
	read(&head, filePath);
	return 0;
}

int Push(Position Head, double value)
{
	Position NewEl;
	NewEl = (Position)malloc(sizeof(postfix));
	if (NewEl == NULL)
	{
		printf("Error allocating memory");
		exit(1);
	}
	NewEl->number = value;

	NewEl->Next = Head->Next;
	Head->Next = NewEl;
	return EXIT_SUCCESS;
}
double Pop(Position Head)
{
	double value;
	if (isEmpty(Head)) {
		printf("Error, nothing to pop!");
		exit(1);
	}
	else
	{
		Position ToDelete = Head->Next;
		Head->Next = ToDelete->Next;
		value = ToDelete->number;
		free(ToDelete);
		return value;
	}
}
int read(Position Head, const char* filePath)
{
	FILE* fp = fopen(filePath, "r");
	if (fp == NULL)
	{
		printf("Error reading file");
		return EXIT_FAILURE;
	}
	int NumBytes;
	double k, operator1, operator2, result;
	char op = '\0';
	char buffer[1024];
	char* ptr;
	fgets(buffer, sizeof(buffer), fp);
	ptr = buffer;
	while (*ptr != '\0')
	{
		if (sscanf(ptr, "%lf %n", &k, &NumBytes) == 1)
		{
			ptr = ptr + NumBytes;
			Push(Head, k);
		}
		else if (sscanf(ptr, "%c %n", &op, &NumBytes) == 1) {
			ptr = ptr + NumBytes;
			operator2 = Pop(Head);
			operator1 = Pop(Head);
			if (op == '+')
				result = operator1 + operator2;
			else if (op == '-')
				result = operator1 - operator2;
			else if (op == '*')
				result = operator1 * operator2;
			else if (op == '/' && operator2 != 0)
				result = operator1 / operator2;
			else
				printf("Error proccessing operands!");
			Push(Head, result);
		}
	}
	double finalResult = Pop(Head);
	if (isEmpty(Head))
		printf("Final result is: %lf\n", finalResult);
	else
		printf("Error");
	fclose(fp);
	return 0;
}
bool isEmpty(Position Head)
{
	if (Head->Next == NULL)
		return true;
	else
		return false;
}