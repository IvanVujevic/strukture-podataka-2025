#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define MAX_LINE 1024
#define MAX_NAME 50


typedef struct _town* townPosition;
typedef struct _town {
	char townName[MAX_NAME];
	int population;
	townPosition left;
	townPosition right;
}Town;

typedef struct _country* countryPosition;
typedef struct _country {
	char countryName[MAX_NAME];
	townPosition TownTree;
	countryPosition next;
}Country;


int createCountry(countryPosition P, char* name, townPosition townRoot);
int readCountryFile(countryPosition C, const char*filename);
townPosition readTownFile(const char*filename);
townPosition createTown(townPosition root, char* name, int val);
int deleteCountry(countryPosition C);
int deleteTown(townPosition root);
int printCountry(countryPosition C);
int printTown(townPosition root);
int menu(countryPosition C);
countryPosition findCountry(countryPosition C, const char* name);
int searchTownByPopulation(townPosition root, int minPopulation);

int main() {
	Country head = { .countryName = {0}, .TownTree=NULL, .next=NULL };
	readCountryFile(&head, "drzave.txt");

	printCountry(head.next);
	menu(head.next);
	deleteCountry(&head);
	return EXIT_SUCCESS;
}

int readCountryFile(countryPosition C, const char*filename) {
	char countryName[MAX_NAME] = {0};
	char FilePointer[MAX_NAME] = { 0 };
	
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return FILE_ERROR_OPEN;
	}

	while (fscanf(fp, "%s %s", countryName, FilePointer)==2) {
		townPosition townRoot = readTownFile(FilePointer);

		createCountry(C, countryName, townRoot);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

townPosition readTownFile(const char*filename) {
	char townName[MAX_NAME] = {0};
	int population=0;
	townPosition root = NULL;

	FILE* fpp = fopen(filename, "r");
	if (!fpp) {
		return NULL;
	}

	while (fscanf(fpp, "%s, %d", townName, &population) == 2) {
		root=createTown(root, townName, population);
	}

	fclose(fpp);
	return root;
}

townPosition createTown(townPosition root, char* name, int val) {
	if (root == NULL) {
		root = (townPosition)malloc(sizeof(Town));
		if (!root) {
			return NULL;
		}
		strcpy(root->townName, name);
		root->population = val;
		root->right = NULL;
		root->left = NULL;
	}
	else if (val < root->population) {
		root->left = createTown(root->left, name, val);
	}
	else if (val >= root->population) {
		root->right = createTown(root->right, name, val);
	}

	return root;
}

int createCountry(countryPosition P, char*name, townPosition townRoot) {
	countryPosition newCountry = (countryPosition)malloc(sizeof(Country));
	if (!newCountry) {
		return MALLOC_ERROR;
	}
	strcpy(newCountry->countryName, name);
	newCountry->TownTree = townRoot;
	newCountry->next = NULL;

	countryPosition prev = P;

	while (prev->next != NULL &&
		strcmp(prev->next->countryName, name) < 0) {
		prev = prev->next;
	}

	newCountry->next = prev->next;
	prev->next = newCountry;

	return EXIT_SUCCESS;
}

int deleteCountry(countryPosition C) {
	countryPosition temp = NULL;
	C = C->next;
	while (C != NULL) {
		deleteTown(C->TownTree);
		temp = C;
		C = C->next;
		temp->next = NULL;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int deleteTown(townPosition root) {
	if (root == NULL) return EXIT_SUCCESS;

	deleteTown(root->left);
	deleteTown(root->right);
	free(root);

	return EXIT_SUCCESS;
}

int printCountry(countryPosition C) {
	while (C != NULL) {
		printf("Country: %s\n", C->countryName);

		if (C->TownTree != NULL)
			printTown(C->TownTree);
		else
			printf(" No towns\n");

		printf("\n");
		C = C->next;
	}
	return EXIT_SUCCESS;
}

int printTown(townPosition root) {
	if (root == NULL)
		return EXIT_SUCCESS;

	printTown(root->left);
	printf(" %s (%d)\n", root->townName, root->population);
	printTown(root->right);

	return EXIT_SUCCESS;

}

countryPosition findCountry(countryPosition C, const char* name) {
	while (C != NULL) {
		if (strcmp(C->countryName, name) == 0)
			return C;
		C = C->next;
	}
	return NULL;
}

int searchTownByPopulation(townPosition root, int minPopulation) {
	if (root == NULL)
		return;

	if (root->population > minPopulation) {
		searchTownByPopulation(root->left, minPopulation);
		printf(" %s (%d)\n", root->townName, root->population);
		searchTownByPopulation(root->right, minPopulation);
	}
	else {
		searchTownByPopulation(root->right, minPopulation);
	}
	return EXIT_SUCCESS;
}

int menu(countryPosition C) {
	char countryName[MAX_NAME] = { 0 };
	int minPopulation=0;

	printf("Enter country name: ");
	scanf("%s", countryName);

	printf("Enter minimum population: ");
	scanf("%d", &minPopulation);

	countryPosition country = findCountry(C, countryName);
	if (!country) {
		printf("Country not found!\n");
		return;
	}

	printf("\nTowns in %s with population > %d:\n",
		country->countryName, minPopulation);

	searchTownByPopulation(country->TownTree, minPopulation);

	return EXIT_SUCCESS;
}

