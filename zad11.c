#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define MAX_LINE 1024
#define MAX_NAME 50
#define TABLE_SIZE 11


typedef struct _town* townPosition;
typedef struct _town {
	char townName[MAX_NAME];
	int population;
	townPosition left;
	townPosition right;
}Town;

typedef struct _country* List;
typedef struct _country* countryPosition;
typedef struct _country {
	char countryName[MAX_NAME];
	int key;
	townPosition TownTree;
	countryPosition next;
}Country;

typedef struct hashT* hashTab;
typedef struct hashT {
	int size;
	countryPosition* hashList;
};

int createCountry(hashTab H, char* name, townPosition townRoot);
townPosition readTownFile(const char*filename);
int readCountryFile(hashTab H, const char*filename);
townPosition createTown(townPosition root, char* name, int val);
int deleteTown(townPosition root);

int printCountry(hashTab H);
int printTown(townPosition root);

countryPosition findCountry(hashTab H, const char* name);
int printTownsAbove(townPosition root, int minPop);
int searchCountryAndTowns(hashTab H, const char* countryName, int minPop);
int findKey(const char* name);
hashTab createTable(int tSize);
int deleteTable(hashTab H);

int main() {
	hashTab HashTable=createTable(TABLE_SIZE);

	readCountryFile(HashTable, "drzave.txt");
	printCountry(HashTable);
	
	char searched[MAX_NAME] = { 0 };
	int population = 0;
	printf("\nWhich country do you want to search: ");
	scanf("%s", searched);
	printf("\nInsert minimum population: ");
	scanf("%d", &population);

	searchCountryAndTowns(HashTable, searched, population);

	deleteTable(HashTable);
	HashTable = NULL;
	return EXIT_SUCCESS;
}

int readCountryFile(hashTab H, const char* filename) {
	char countryName[MAX_NAME] = { 0 };
	char FilePointer[MAX_NAME] = { 0 };

	FILE* fp = fopen(filename, "r");
	if (!fp) {
		return FILE_ERROR_OPEN;
	}

	while (fscanf(fp, "%s %s", countryName, FilePointer) == 2) {
		townPosition townRoot = readTownFile(FilePointer);

		createCountry(H, countryName, townRoot);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

townPosition readTownFile(const char* filename) {
	char townName[MAX_NAME] = { 0 };
	int population = 0;
	townPosition root = NULL;

	FILE* fpp = fopen(filename, "r");
	if (!fpp) {
		return NULL;
	}

	while (fscanf(fpp, "%s %d", townName, &population) == 2) {
		root = createTown(root, townName, population);
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

int createCountry(hashTab H, char* name, townPosition townRoot) {
	countryPosition newCountry = (countryPosition)malloc(sizeof(Country));
	if (!newCountry) {
		return MALLOC_ERROR;
	}
	strcpy(newCountry->countryName, name);
	newCountry->TownTree = townRoot;
	newCountry->next = NULL;
	newCountry->key = findKey(name);

	int index = newCountry->key % H->size;

	countryPosition curr = H->hashList[index];
	countryPosition prev = NULL;

	while (curr != NULL && strcmp(curr->countryName, name) < 0) {
		prev = curr;
		curr = curr->next;
	}

	if (prev == NULL) {
		newCountry->next = H->hashList[index];
		H->hashList[index] = newCountry;
	}
	else {
		newCountry->next = curr;
		prev->next = newCountry;
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

int printCountry(hashTab H) {
	if (H == NULL) {
		printf("Hash table is empty.\n");
		return NULL;
	}

	for (int i = 0; i < H->size;i++) {
		printf("Level %d", i+1);

		countryPosition curr = H->hashList[i];
		if (curr == NULL) {
			printf(" \n	(empty)\n");
			continue;
		}

		while (curr != NULL) {
			printf(" \nCountry: %s (%d)\n", curr->countryName, curr->key);

			if (curr->TownTree != NULL) {
				printf("  Towns:\n");
				printTown(curr->TownTree);
			}
			else {
				printf("  No towns\n");
			}

			curr = curr->next;
		}
			
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

hashTab createTable(int tSize) {
	hashTab H;
	int i;
	H = (hashTab)malloc(sizeof(struct hashT));
	if (H==NULL) {
		printf("\nMalloc error!\n");
		return NULL;
	}
	H->size = tSize;
	H->hashList = (List*)malloc(sizeof(List)*H->size);
	if (H->hashList == NULL) {
		printf("\nMalloc error");
		return NULL;
	}

	for (i = 0; i < H->size; i++) {
		H->hashList[i] = NULL;
	}

	return H;
}

int deleteTable(hashTab H)
{
	if (H == NULL)
		return 0;

	for (int i = 0; i < H->size; i++) {
		countryPosition curr = H->hashList[i];

		while (curr != NULL) {
			countryPosition temp = curr;
			curr = curr->next;

			deleteTown(temp->TownTree);

			free(temp);
		}
	}

	free(H->hashList);
	free(H);

	return EXIT_SUCCESS;
}

int findKey(const char* name) {
	int newKey = (name[0]+name[1]+name[2]+name[3]+name[4]) % TABLE_SIZE;
	return newKey;
}

int searchCountryAndTowns(hashTab H, const char* countryName, int minPop){
	countryPosition country = findCountry(H, countryName);

	if (country == NULL) {
		printf("Country '%s' not found.\n", countryName);
		return EXIT_FAILURE;
	}

	printf("\nCountry: %s\n", country->countryName);
	printf("Towns with population greater than %d:\n", minPop);

	if (country->TownTree == NULL) {
		printf(" No towns available.\n");
		return EXIT_SUCCESS;
	}

	printTownsAbove(country->TownTree, minPop);

	return EXIT_SUCCESS;
}

int printTownsAbove(townPosition root, int minPop){
	if (root == NULL)
		return EXIT_SUCCESS;

	printTownsAbove(root->left, minPop);

	if (root->population > minPop) {
		printf(" %s (%d)\n", root->townName, root->population);
	}

	printTownsAbove(root->right, minPop);

	return EXIT_SUCCESS;
}
countryPosition findCountry(hashTab H, const char* name){
	if (!H || !name)
		return NULL;

	int key = findKey(name);
	int index = key % H->size;

	countryPosition curr = H->hashList[index];

	while (curr != NULL) {
		if (strcmp(curr->countryName, name) == 0)
			return curr;
		curr = curr->next;
	}

	return NULL;
}