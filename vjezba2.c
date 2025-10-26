#define _CRT_SECURE_NO_WARNINGS
#define SCANF_ERROR -2
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 50

typedef struct _person* position;
typedef struct _person {
	char name[MAX];
	char surname[MAX];
	int birthyear;
	position next;
}person;

position createPerson(char*, char*, int);
int addToFront(position, char* name, char* surname, int birthyear);
int printList(position);
int addToEnd(position, char* name, char* surname, int birthyear);
int searchBySurname(position, char*);
int removePerson(position, char*);
int removeList(position);
position findPrev(position, char*);

int main() {
	person head = { .name = {0}, .surname = {0}, .birthyear = 0, .next = NULL };
	char ch = 0;
	char inputName[MAX] = {0};
	char inputSurname[MAX] = {0};
	int inputBirthyear = 0;

	addToFront(&head, "Ivan", "Ivic", 2000);
	addToEnd(&head, "Ana", "Anic", 2001);
	printf("a - add to the front of the list\nb - print out the list\nc - add to the end of the list\nd - search by surname in the list\ne - delete element from the list\n0 - end program\n");

	do {
		printf("Which of the following actions do you want to execute: ");
		if (scanf(" %c", &ch) != 1) return SCANF_ERROR;

		switch (ch) {

		case 'a':
			printf("add to the front of the list:\nInsert name, surname and birthyear: ");
			scanf("%s %s %d", &inputName, &inputSurname, &inputBirthyear);
			addToFront(&head, inputName, inputSurname, inputBirthyear);
			break;

		case 'b':
			printf("\nprint out the list\n");
			printList(&head);
			break;

		case 'c':
			printf("add to the end of the list:\nInsert name, surname and birthyear: ");
			scanf("%s %s %d", &inputName, &inputSurname, &inputBirthyear);
			addToEnd(&head, inputName, inputSurname, inputBirthyear);
			break;

		case 'd':
			printf("search by surname in the list");
			scanf("%s", &inputSurname);
			searchBySurname(&head, inputSurname);
			break;

		case 'e':
			printf("delete element from the list");
			scanf("%s", &inputSurname);
			removePerson(&head, inputSurname);
			break;

		case '0':
			break;

		default:
			printf("Invalid input");
			break;
		}
	} while (ch != '0');


	removeList(&head);
	return 1;
}

position createPerson(char* name, char* surname, int birthyear) {
	position newPerson = (position)malloc(sizeof(person));
	if (newPerson == NULL) {
		printf("Malloc error!");
		return NULL;
	}
	strncpy(newPerson->name, name, sizeof(newPerson->name));
	strncpy(newPerson->surname, surname, sizeof(newPerson->surname));
	newPerson->birthyear = birthyear;
	newPerson->next = NULL;
	return newPerson;
}

int addToFront(position head, char* name, char* surname, int birthyear) {
	position p = createPerson(name, surname, birthyear);
	p->next = head->next;
	head->next = p;
	return EXIT_SUCCESS;
}

int printList(position head) {
	position start = head->next;
	while (start != NULL) {
		printf("\n%s\t%s\t%d\n", start->name, start->surname, start->birthyear);
		start = start->next;
	}
	return EXIT_SUCCESS;
}

int addToEnd(position head, char* name, char* surname, int birthyear) {
	position p = createPerson(name, surname, birthyear);
	while (head->next != NULL) {
		head = head->next;
	}
	p->next = head->next;
	head->next = p;
	return EXIT_SUCCESS;
}

int searchBySurname(position head, char* surname) {
	position start = head->next;
	while (start != NULL) {
		if (strcmp(start->surname, surname) == 0) {
			printf("%s\t%s\t%d\n", start->name, start->surname, start->birthyear);
		}
		start = start->next;
	}
	return EXIT_SUCCESS;
}

int removePerson(position head, char* surname) {
	position prev = findPrev(head, surname);
	position temp = NULL;

	if (prev != NULL) {
		temp = prev->next;
		prev->next = temp->next;
		free(temp);
	}
	else printf("Person not found");

	return EXIT_SUCCESS;
}

position findPrev(position current, char* surname) {
	while (current->next != NULL && strcmp(current->next->surname, surname) != 0)
		current = current->next;

	if (current->next == NULL) return NULL;

	return current;
}

int removeList(position head) {
	position start = head->next;
	position toDelete = NULL;
	while (start != NULL) {
		toDelete = start;
		start = start->next;
		free(toDelete);
	}

	return EXIT_SUCCESS;

}

