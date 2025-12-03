#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 128
#define MAX 50

#define EXIT_SUCCESS 0
#define FILE_OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3


typedef struct _article* PositionA;
typedef struct _article {
	char ArticleName[MAX_NAME];
	int amount;
	double price;
	PositionA next;
}Article;

typedef struct _receipt* PositionR;
typedef struct _receipt {
	int year, month, day;
	Article ArticleHead;
	PositionR next;
}Receipt;

PositionR createReceipt(char*);
PositionA createArticle(char*, int, double);
int printList(PositionR);
int freeList(PositionR);

int main() {

	Receipt head;
	head.next = NULL;
	FILE* fp = NULL;

	fp = fopen("racuni.txt", "r");
	if (fp == NULL) {
		printf("Error opening main file");
		return;
	}

	char line[MAX];
	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0';
		PositionR temp = createReceipt(line);
		insertSortReceipt(head, temp);
	}
	printList(&head);
	printf("Which article do you want to search?\n");
	char search[MAX];
	scanf("%s", search);


	fclose(fp);
	return EXIT_SUCCESS;
}

PositionR createReceipt(char* filePath)
{
	PositionR newReceipt = (PositionR)malloc(sizeof(Receipt));
	if (newReceipt == NULL) {
		printf("Error creating receipt");
		return NULL;
	}
	FILE* file = fopen(filePath, "r");
	if (file == NULL) {
		printf("Error opening file in receipt");
		return NULL;
	}
	int year, month, day, amount;
	double price;
	char article[50], line[50];
	fgets(line, sizeof(line), file);
	if (sscanf(line, "%4d-%2d-%2d", &year, &month, &day) == 3) {
		newReceipt->year = year;
		newReceipt->month = month;
		newReceipt->day = day;
	}
	else {
		printf("Error reading date");
		return NULL;
	}
	newReceipt->ArticleHead.next = NULL;
	newReceipt->next = NULL;
	while (fgets(line, sizeof(line), file)) {
		if (sscanf(line, "%s %d %lf", article, &amount, &price) == 3) {
			PositionA temp = createArticle(article, amount, price);
			insertSortArticle(&newReceipt->ArticleHead, temp);
		}
		else {
			printf("Error reading articles");
			return NULL;
		}
	}
	fclose(file);
	return newReceipt;
}

PositionA createArticle(char* article, int amount, double price) {
	PositionA newArticle = (PositionA)malloc(sizeof(Article));
	if (newArticle == NULL) {
		printf("New article error");
		return NULL;
	}
	strcpy(newArticle->ArticleName, article);
	newArticle->amount = amount;
	newArticle->price = price;
	newArticle->next = NULL;

	return newArticle;
}

int insertSortRec(PositionR head, PositionR P)
{
	PositionR temp = head;
	while (temp->next != NULL && !compareDates(temp->next, P))
		temp = temp->next;
	P->next = temp->next;
	temp->next = P;


	return EXIT_SUCCESS;
}

int insertSortArt(PositionA head, PositionA P)
{
	PositionA temp = head;
	while (temp->next != NULL && !compareStrings(temp->next, P))
		temp = temp->next;
	P->next = temp->next;
	temp->next = P;


	return EXIT_SUCCESS;
}

int printList(PositionR head)
{
	PositionR temp = head->next;
	while (temp != NULL) {
		printf("Na dan %d.%d.%d. prodano je:\n", temp->day, temp->month, temp->year);
		PositionA tempArt = temp->ArticleHead.next;
		while (tempArt != NULL) {
			printf("%s, %d komada po %.2lf eura\n", tempArt->ArticleName, tempArt->amount, tempArt->price);
			tempArt = tempArt->next;
		}
		printf("\n");
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int freeList(PositionR head){
	PositionR temp = head->next;
	while (temp != NULL) {
		PositionA tempArt = temp->ArticleHead.next;
		while (tempArt != NULL) {
			PositionA nextArt = tempArt->next;
			free(tempArt);
			tempArt = nextArt;
		}
		PositionR next = temp->next;
		free(temp);
		temp = next;
	}
	return EXIT_SUCCESS;
}