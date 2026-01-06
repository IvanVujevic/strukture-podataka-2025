#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MALLOC_ERROR -1
#define FILE_OPEN_ERROR -2
#define MAX 90
#define MIN 10

typedef struct tree* Position;
typedef struct tree {
	int num;
	Position left;
	Position right;
}Tree;

Position insert(Position root, int val);
int generateRandNum();
int deleteTree(Position root);
int print(Position root);
int replace(Position root);
int writeInOrderToFile(Position root, FILE*fp);


int main() {
	srand(time(NULL));
	Position root = NULL;

	root=insert(root, 2);
	root=insert(root, 5);
	root = insert(root, 7);
	root = insert(root, 8);
	root = insert(root, 11);
	root = insert(root, 1);
	root = insert(root, 4);
	root = insert(root, 2);
	root = insert(root, 3);
	root = insert(root, 7);

	print(root);
	replace(root);
	printf("\n");
	print(root);

	FILE* fp = fopen("dat.txt", "w");
	if (!fp) {
		return FILE_OPEN_ERROR;
	}
	for (int i = 0; i < 10; i++) {
		int br = generateRandNum();
		root=insert(root, br);
	}

	writeInOrderToFile(root, fp);
	fclose(fp);
	
	deleteTree(root);
	return EXIT_SUCCESS;
}

Position insert(Position root, int val) {
	if (root == NULL) {
		root = (Position)malloc(sizeof(Tree));
		if (!root) {
			return NULL;
		}
		root->num = val;
		root->right = NULL;
		root->left = NULL;
	}
	else if (val < root->num) {
		root->left = insert(root->left, val);
	}
	else if (val >= root->num) {
		root->right = insert(root->right, val);
	}

	return root;
}


int deleteTree(Position root) {

	if (root == NULL) return EXIT_SUCCESS;

	deleteTree(root->left);
	deleteTree(root->right);
	free(root);

	return EXIT_SUCCESS;
}

int replace(Position root)
{
	if (root == NULL)
		return 0;

	int leftSum = replace(root->left);
	int rightSum = replace(root->right);

	int oldValue = root->num;

	root->num = leftSum + rightSum;

	return oldValue + root->num;
}

int print(Position root) {
	if (root == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		print(root->left);
		printf("%d ", root->num);
		print(root->right);
	}
	return EXIT_SUCCESS;
}

int writeInOrderToFile(Position root, FILE* fp){
	if (root == NULL)
		return EXIT_SUCCESS;

	writeInOrderToFile(root->left, fp);
	fprintf(fp, "%d ", root->num);
	writeInOrderToFile(root->right, fp);

	return EXIT_SUCCESS;
}

int generateRandNum() {
	int i = 0;
	i = (rand() % (MAX - MIN + 1)) + MIN;
	return i;
}