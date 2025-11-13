#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ERROR_OPEN -4
#define MAX_LINE 1024
#define MAX_SIZE 100


typedef struct _polynom* position;
typedef struct _polynom {
	int coef;
	int exp;
	position next;
} poly;

position createElement(int, int);
int freeList(position head);
int insertSort(position head, const char* filePath);
int addAfter(position new, position old);
int deleteElement(position head, position delete);
position findPrev(position head, position P);
int printList(position head);
int multiply(position head1, position head2, position head3);
int addition(position head1, position head2, position head3);


int main() {
	poly head1, head2, head3 = { .next = NULL };

	insertSort(&head1, "polynom1.txt");
	insertSort(&head2, "polynom2.txt");
	printList(&head1);
	printList(&head2);
	printf("\nMultiplyed:\n");
	multiply(&head1, &head2, &head3);
	printList(&head3);
	freeList(&head3);
	printf("\nAddition:\n");
	addition(&head1, &head2, &head3);
	printList(&head3);
	freeList(&head3);

	freeList(&head1);
	freeList(&head2);

	return EXIT_SUCCESS;
}

position createElement(int co, int expo) {

	position newEl = (position)malloc(sizeof(poly));
	if (newEl == NULL){
		printf("Malloc error");
		return NULL;
	}

	newEl->coef = co;
	newEl->exp = expo;
	newEl->next = NULL;
	return newEl;
}

int insertSort(position head, const char* filepath) {

	position temp = head;
	int a, b, numBytes;
	char buffer[MAX_LINE];
	char* ptr = buffer;

	FILE* fp = fopen(filepath, "r");
	if (fp == NULL) {
		printf("Error");
		return FILE_ERROR_OPEN;
	}
	
	fgets(buffer, sizeof(buffer), fp);

	if (sscanf(ptr,"%d %d %n", &a, &b, &numBytes)==2) {
		position newElement = createElement(a, b);
		ptr += numBytes;

		while (temp->next && temp->exp < newElement->exp) {
			temp = temp->next;
		}

		if (temp && temp->exp != newElement->exp)  
			addAfter(temp, newElement);

		else {
			int newCoef = temp->coef + newElement->coef;
			if (newCoef == 0) {
				deleteElement(head, temp);
				newElement->next = NULL;
				free(newElement);
			}
			else 
				temp->coef = newCoef;
			
		}
	}
	else
		printf("Error");

	fclose(fp);
	return EXIT_SUCCESS;
}

int addAfter(position new, position old) {
	new->next = old->next;
	old->next = new;

	return EXIT_SUCCESS;
}

int deleteElement(position head, position toDelete) {
	position prev = findPrev(head, toDelete);
	prev->next = toDelete->next;
	toDelete->next = NULL;
	free(toDelete);

	return EXIT_SUCCESS;
}

position findPrev(position head, position P) {
	position temp = head;
	while (temp != P)
		temp = temp->next;
	return temp;
}

int printList(position head) {
	position temp = head->next;
	while (temp != NULL) {
		printf("%dx^%d ", temp->coef, temp->exp);
		temp = temp->next;
	}
	printf("\n");

	return EXIT_SUCCESS;
}

int multiply(position head1, position head2, position head3) {
	position temp1 = head1->next;
	position temp2 = head2->next;
	position temp3 = head3;
	while (temp1 != NULL) {
		temp2 = head2->next;
		while (temp2 != NULL) {
			int newCoef = temp1->coef * temp2->coef;
			int newExp = temp1->exp + temp2->exp;
			position newElement = createElement(newCoef, newExp);
			addAfter(temp3, newElement);
			temp3 = temp3->next;
			temp2 = temp2->next;
		}
		temp1 = temp1->next;
	}


	return EXIT_SUCCESS;
}

int addition(position head1, position head2, position head3) {
	position temp1 = head1->next;
	position temp2 = head2->next;
	position temp3 = head3;
	int tracker = 0;
	while (temp1 != NULL) {
		while (temp2 != NULL) {
			if (temp1->exp == temp2->exp) {
				tracker = 1;
				int newCoef = temp1->coef + temp2->coef;
				position newElement = createElement(newCoef, temp1->exp);
				addAfter(temp3, newElement);
				temp3 = temp3->next;
				temp1 = temp1->next;
				if (temp1 == NULL && temp2 != NULL) {
					while (temp2 != NULL) {
						addAfter(temp3, temp2);
						temp3 = temp3->next;
						temp2 = temp2->next;
					}
				}
			}
			else {
				tracker = 0;
				temp2 = temp2->next;
			}
		}
		if (tracker == 0) {
			addAfter(temp3, temp1);
			temp3 = temp3->next;
			temp1 = temp1->next;
		}
	}

	return EXIT_SUCCESS;
}

int freeList(position head){
	position temp = head;
	while (temp != NULL) {
		position next = temp->next;
		free(temp);
		temp = next;
	}
	return EXIT_SUCCESS;
}