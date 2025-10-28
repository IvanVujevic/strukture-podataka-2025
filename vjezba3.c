#define _CRT_SECURE_NO_WARNINGS
#define SCANF_ERROR -2
#define MALLOC_ERROR -3
#define FILE_ERROR_OPEN -4
#define MAX 50
#define MAX_LINE 1024

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
position findCurrent(position, char*);
int addBefore(position, char* name, char* surname, int birthyear, char*);
int addBehind(position, char* name, char* surname, int birthyear, char*);
int sortList(position);
int writeIntoFile(position);
int readFromFile(position);

int main() {
	person head = { .name = {0}, .surname = {0}, .birthyear = 0, .next = NULL };
	char ch = 0;
	char inputName[MAX] = { 0 };
	char inputSurname[MAX] = { 0 };
	char searchSurname[MAX] = { 0 };
	int inputBirthyear = 0;

	addToFront(&head, "Ivan", "Ivic", 2000);
	addToEnd(&head, "Ana", "Anic", 2001);
	printf("a - add to the front of the list\nb - print out the list\nc - add to the end of the list\nd - search by surname in the list\ne - delete element from the list\nf - add before element\ng - add after element\nh - sort the list\ni - write into the file\nj - read list from file\n0 - end program\n");

	do {
		printf("Which of the following actions do you want to execute: ");
		if (scanf(" %c", &ch) != 1) return SCANF_ERROR;

		switch (ch) {

		case 'a':
			printf("add to the front of the list:\nInsert name, surname and birthyear of new elemnt: ");
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

		case 'f':
			printf("add before element:\nInsert name, surname and birthyear: ");
			scanf("%s %s %d", &inputName, &inputSurname, &inputBirthyear);
			printf("\nInsert surname of the element in the list: ");
			scanf("%s", &searchSurname);
			addBefore(&head, inputName, inputSurname, inputBirthyear, searchSurname);
			break;

		case 'g':
			printf("add after element:\nInsert name, surname and birthyear: ");
			scanf("%s %s %d", &inputName, &inputSurname, &inputBirthyear);
			printf("\nInsert surname of the element in the list: ");
			scanf("%s", &searchSurname);
			addBehind(&head, inputName, inputSurname, inputBirthyear, searchSurname);
			break;

		case 'h':
			printf("sort the list\n");
			sortList(&head);
			break;

		case 'i':
			printf("write into the file\n");
			writeIntoFile(head.next);
			break;

		case 'j':
			printf("read list from file\n");
			readFromFile(&head);
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
	int found = 0;
	while (start != NULL) {
		if (strcmp(start->surname, surname) == 0) {
			printf("%s\t%s\t%d\n", start->name, start->surname, start->birthyear);
			found = 1;
		}
		start = start->next;
	}

	if (!found)printf("Person not found");
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

position findCurrent(position head, char* surname) {
	while (head != NULL && strcmp(head->surname, surname))
		head = head->next;

	if (head == NULL) return NULL;

	return head;
}

int addBefore(position head, char* name, char* surname, int birthyear, char* ssurname) {
	position newPerson = NULL;
	position searchedSurname = NULL;
	searchedSurname = findPrev(head, ssurname);

	if (searchedSurname != NULL) {
		newPerson = createPerson(name, surname, birthyear);
		if (!newPerson) {
			printf("Malloc error!");
			return MALLOC_ERROR;
		}
		newPerson->next = searchedSurname->next;
		searchedSurname->next = newPerson;
	}
	else printf("person not found\n");

	return EXIT_SUCCESS;
}

int addBehind(position head, char* name, char* surname, int birthyear, char* ssurname) {
	position newPerson = NULL;
	position searchedSurname = NULL;
	searchedSurname = findCurrent(head, ssurname);

	if (searchedSurname != NULL) {
		newPerson = createPerson(name, surname, birthyear);
		if (!newPerson) {
			printf("Malloc error!");
			return MALLOC_ERROR;
		}
		newPerson->next = searchedSurname->next;
		searchedSurname->next = newPerson;
	}
	else printf("person not found\n");

	return EXIT_SUCCESS;
}

int sortList(position head) {
	position j, prev_j, temp, end;
	end = NULL;
	while (head->next != end) {
		prev_j = head;
		j = head->next;
		while (j->next != end) {
			if (strcmp(j->surname, j->next->surname) > 0) {
				temp = j->next;
				prev_j->next = temp;
				j->next = temp->next;
				temp->next = j;

				j = temp;
			}
			prev_j = j;
			j = j->next;
		}
		end = j;
	}

	return EXIT_SUCCESS;
}

int writeIntoFile(position current) {
	FILE* fp = NULL;
	fp = fopen("people.txt", "w");

	if (fp == NULL) {
		printf("File not opened.");
		return FILE_ERROR_OPEN;
	}

	while (current != NULL) {
		fprintf(fp, "%s %s %d\n", current->name, current->surname, current->birthyear);
		current = current->next;
	}

	fclose(fp);
	return EXIT_SUCCESS;
}

int readFromFile(position head) {

	char person[MAX_LINE] = { 0 };
	char name[MAX], surname[MAX] = { 0 };
	int year;
	position temp = head;

	FILE* fp;
	fp = fopen("people.txt", "r");

	if (fp == NULL) {
		printf("File not opened.");
		return FILE_ERROR_OPEN;
	}

	while (fgets(person, sizeof(person), fp) != NULL) {
		if (sscanf(person, "%s %s %d", &name, &surname, &year) == 3) {
			position P = createPerson(name, surname, year);
			P->next = temp->next;
			temp->next = P;
			temp = temp->next;
		}
	}


	fclose(fp);
	return EXIT_SUCCESS;
}