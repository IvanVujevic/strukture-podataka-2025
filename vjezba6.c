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
	char date[11];
	Article ArticleHead;
	PositionR next;
}Receipt;

PositionR createReceipt(char*);
PositionA createArticle(char*, int, double);
int insertSortRec(PositionR, PositionR);
int insertSortArt(PositionA, PositionA);
int printArticles(PositionA);
int printReceipts(PositionR);
int freeList(PositionR);
int getArticleInPeriod(PositionR currentReceipt, char* article, char* startDate, char* endDate, int* income, int* count);

int main() {

	int income = 0, count = 0;
	char search[MAX] = { 0 };
	char startDate[20] = { 0 };
	char endDate[20] = { 0 };
	Receipt head;
	head.next = NULL;
	Receipt date = { 0 };
	FILE* fp = NULL;

	fp = fopen("racuni.txt", "r");
	if (fp == NULL) {
		printf("Error opening main file");
		return FILE_OPEN_ERROR;
	}

	char line[MAX];
	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0';
		PositionR temp = createReceipt(line);
		insertSortRec(&head, temp);
	}
	printReceipts(head.next);

	printf("\nEnter article name: ");
	if (scanf("%s", search) != 1) return SCANF_ERROR;
	printf("Enter start date (YYYY-MM-DD): ");
	if (scanf("%s", startDate) != 1) return SCANF_ERROR;
	printf("Enter end date (YYYY-MM-DD): ");
	if (scanf("%s", endDate) != 1) return SCANF_ERROR;

	getArticleInPeriod(head.next, search, startDate, endDate, &income, &count);
	printf("Income of %d %s articles: %d\n", count, search, income);

	fclose(fp);
	freeList(&head);
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
	int amount=0;
	double price=0.0;
	char article[50], line[50];


	fgets(line, 50, file);
	line[strcspn(line, "\n")] = 0;
	strcpy(newReceipt->date, line);

	newReceipt->ArticleHead.next = NULL;
	newReceipt->next = NULL;
	while (fgets(line, sizeof(line), file)) {
		if (sscanf(line, "%s, %d, %lf", article, &amount, &price) == 3) {
			PositionA temp = createArticle(article, amount, price);
			insertSortArt(&newReceipt->ArticleHead, temp);
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
	while (temp->next != NULL && strcmp(P->date, temp->next->date) >= 0) {
		temp = temp->next;
	}

	P->next = temp->next;
	temp->next = P;
	
	return EXIT_SUCCESS;
}



int insertSortArt(PositionA head, PositionA P)
{
	PositionA temp = head;
	while (temp->next != NULL && strcmp(P->ArticleName, temp->next->ArticleName) >= 0)
		temp = temp->next;
	P->next = temp->next;
	temp->next = P;


	return EXIT_SUCCESS;
}


int getArticleInPeriod(PositionR currentReceipt, char* search, char* startDate, char* endDate, int* income, int* count)
{
	PositionA currentArticle = NULL;

	while (currentReceipt != NULL && strcmp(currentReceipt->date, startDate) < 0)
		currentReceipt = currentReceipt->next;

	while (currentReceipt != NULL && strcmp(currentReceipt->date, endDate) <= 0) {
		currentArticle = currentReceipt->ArticleHead.next;
		while (currentArticle != NULL) {
			if (strcmp(currentArticle->ArticleName, search) == 0) {
				*income += currentArticle->amount * currentArticle->price;
				*count += currentArticle->amount;
			}
			currentArticle = currentArticle->next;
		}
		currentReceipt = currentReceipt->next;
	}

	return EXIT_SUCCESS;
}

int printArticles(PositionA current)
{
	while (current != NULL) {
		printf("\t%s, %d, %.2lf\n", current->ArticleName, current->amount, current->price);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int printReceipts(PositionR current)
{
	while (current != NULL) {
		printf("Receipt from date %s:\n", current->date);
		printArticles(current->ArticleHead.next);
		current = current->next;
	}

	return EXIT_SUCCESS;
}

int freeList(PositionR head) {
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
