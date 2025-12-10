#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

#define EMPTY_STACK 1
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1
#define SCANF_ERROR -2

typedef struct _stack* stackPosition;
typedef struct _directory* dirPosition;

typedef struct _directory {
	char name[MAX];
	dirPosition next;
	dirPosition subDirPos;
}Directory;

typedef struct _stack {
	dirPosition dirLevel;
	stackPosition next;
}Stack;

dirPosition createDirectory(char* directoryName);
int push(stackPosition, dirPosition);
int pop(stackPosition);
int addDirectory(dirPosition, dirPosition);
int changeDirectory(stackPosition, char*);
int printDirectory(dirPosition, int);
int deleteDirectory(dirPosition);
int deleteStack(stackPosition);

int main() {

	Directory HeadDirectory = { .name = {0}, .next = NULL, .subDirPos=NULL};
	Stack HeadStack = { .next = NULL, .dirLevel=NULL };
	dirPosition rootDirectory = NULL;
	char directoryName[MAX];
	char c = 0;

	rootDirectory = createDirectory("C:");
	if (rootDirectory == NULL) return MALLOC_ERROR;

	HeadDirectory.next = rootDirectory;
	push(&HeadStack, rootDirectory);

	printf("1 - make directory\n2 - change directory\n3 - go to previous directory\n4 - print out directory\n5 - end program");

	do {
		printf("Choose command: ");
		if (scanf(" %c", &c) != 1) return SCANF_ERROR;

		switch (c) {
		case '1':
			printf("\nName for new directory: ");
			if (scanf("%s", directoryName) != 1) return SCANF_ERROR;
			dirPosition newDirectory = createDirectory(directoryName);
			addDirectory(HeadStack.next->dirLevel, newDirectory);
			break;
		case '2':
			printf("\nName for next directory: ");
			if (scanf(" %s", directoryName) != 1) return SCANF_ERROR;
			changeDirectory(&HeadStack, directoryName);
			break;
		case '3':
			if (pop(&HeadStack)) printf("Already in root directory.\n");
			else printf("You are now in parent directory\n");
			break;
			break;
		case '4':
			printDirectory(HeadStack.next->dirLevel, 0);
			break;
		case '5':
			printf("End program");
			break;
		default:
			printf("Invald input\n");
			break;
		}
	} while (c != '5');

	deleteDirectory(&HeadDirectory);
	deleteStack(&HeadStack);
	return EXIT_SUCCESS;
}

dirPosition createDirectory(char* directoryName) {
	dirPosition newDirectory = (dirPosition)malloc(sizeof(Directory));
	if (newDirectory == NULL) return NULL;

	strcpy(newDirectory->name, directoryName);
	newDirectory->next=NULL;
	newDirectory->subDirPos=NULL;

	return newDirectory;
}

int addDirectory(dirPosition HeadStackNext, dirPosition newDir) {
	dirPosition parentDir = HeadStackNext;
	newDir->next = parentDir->subDirPos;
	parentDir->subDirPos = newDir;

	return EXIT_SUCCESS;
}

int changeDirectory(stackPosition HeadStackAdr, char* directoryName) {
	dirPosition current = HeadStackAdr->next->dirLevel->subDirPos;

	while (current != NULL && strcmp(current->name, directoryName) != 0) current = current->next;

	if (current != NULL) {
		printf("Changed directory to %s\n", directoryName);
		push(HeadStackAdr, current);
	}
	else printf("Directory not found.\n");
	return EXIT_SUCCESS;
}

int push(stackPosition HeadStack, dirPosition currentDirectory) {
	stackPosition newStackLevel = (stackPosition)malloc(sizeof(Stack));
	if (newStackLevel == NULL) return MALLOC_ERROR;

	newStackLevel->dirLevel = currentDirectory;
	newStackLevel->next = HeadStack->next;
	HeadStack->next = newStackLevel;

	return EXIT_SUCCESS;
}

int pop(stackPosition HeadStackAdr) {
	stackPosition temp;
	if (HeadStackAdr->next == NULL) return EMPTY_STACK;

	temp = HeadStackAdr->next;
	HeadStackAdr->next = HeadStackAdr->next->next;
	free(temp);

	return EXIT_SUCCESS;
}

int printDirectory(dirPosition current, int depth) {

	int i = 0;
	dirPosition currentSub = NULL;

	if (current->subDirPos != NULL) {
		currentSub = current->subDirPos;
		while (currentSub != NULL) {
			for (i = 0; i < depth; i++)
				printf(" ");
			printf("%s\n", currentSub->name);
			printDirectory(currentSub, depth + 1);
			currentSub = currentSub->next;
		}
	}

	return 0;
}

int deleteDirectory(dirPosition HeadDirectory) {
	dirPosition temp = NULL;
	while (HeadDirectory->next != NULL){
		temp = HeadDirectory->next;
		HeadDirectory->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int deleteStack(stackPosition HeadStack) {
	stackPosition temp = NULL;
	while (HeadStack->next != NULL) {
		temp = HeadStack->next;
		HeadStack->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}