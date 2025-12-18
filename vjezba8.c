#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define MALLOC_ERROR -1

typedef  struct _tree* treePosition;
typedef struct _tree {
	int El;
	treePosition left;
	treePosition right;
}tree;

treePosition createLeaf(treePosition leaf, int val);
int inorder(treePosition current);
int preorder(treePosition current);
int postorder(treePosition current);
treePosition searchElement(treePosition curr, int val);
treePosition deleteElement(treePosition curr, int val);
treePosition findMin(treePosition curr);
int deleteTree(treePosition curr);


int main() {
	treePosition root = NULL;
	int input,value;

	printf("Choose action:\n1 - insert element\n2 - search element\n3 - delete element\n4 - print inorder\n5 - print preorder\n6 - print postorder\n0 - exit\n");
	
	do{
		scanf("%d", &input);
		switch (input) {
		case 1:
			printf("\nInsert value: ");
			scanf("%d", &value);
			root=createLeaf(root, value);
			break;
		case 3:
			printf("\nInsert element to delete: ");
			scanf("%d", &value);
			root=deleteElement(root, value);
			break;
		case 2:
			printf("\nInsert element to search: ");
			scanf("%d", &value);
			if (searchElement(root, value))
				printf("\nElement found");
			else
				printf("\nElement not found");
			break;
		case 4:
			printf("\nprint inorder: ");
			inorder(root);
			break;
		case 5:
			printf("\nprint preorder: ");
			preorder(root);
			break;
		case 6:
			printf("\nprint postorder: ");
			postorder(root);
			break;
		case 0:
			break;
		default:
			printf("\nInput error");
			break;
		}
	} while (input != 0);

	deleteTree(root);
	return EXIT_SUCCESS;
}

treePosition createLeaf(treePosition leaf, int val) {
	if (leaf == NULL) {
		leaf = (treePosition)malloc(sizeof(tree));
		if (leaf == NULL) {
			printf("Malloc error");
			return NULL;
		}
		leaf->El = val;
		leaf->left = NULL;
		leaf->right = NULL;
	}
	else if (val < leaf->El) {
		leaf->left = createLeaf(leaf->left, val);
	}
	else if (val > leaf->El) {
		leaf->right = createLeaf(leaf->right, val);
	}

	return leaf;
}
int inorder(treePosition current) {
	if (current == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		inorder(current->left);
		printf("%d ", current->El);
		inorder(current->right);
	}
	return EXIT_SUCCESS;
}

int preorder(treePosition current) {
	if (current == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		printf("%d ", current->El);
		preorder(current->left);
		preorder(current->right);
	}
	return EXIT_SUCCESS;
}

int postorder(treePosition current) {
	if (current == NULL) {
		return EXIT_SUCCESS;
	}
	else {
		postorder(current->left);
		postorder(current->right);
		printf("%d ", current->El);
	}
	return EXIT_SUCCESS;
}

treePosition searchElement(treePosition current, int val) {
	if (current == NULL)
		return NULL;
	else if (current->El < val)
		return searchElement(current->right, val);
	else if (current->El > val)
		return searchElement(current->left, val);
	else
	return current;
}

treePosition deleteElement(treePosition curr, int val) {
	if (curr == NULL) {
		return NULL;
	}

	if (val < curr->El)
		curr->left = deleteElement(curr->left, val);
	else if (val > curr->El)
		curr->right = deleteElement(curr->right, val);
	else if (curr->left != NULL && curr->right != NULL) {
		treePosition temp = findMin(curr->right);
		curr->El = temp->El;
		curr->right = deleteElement(curr->right, curr->El);
	}

	else {
		treePosition temp = curr;
		if (curr->left == NULL)
			curr = curr->right;
		else
			curr = curr->left;

		free(temp);
	}

	return curr;
}

treePosition findMin(treePosition curr) {
	if (curr == NULL)
		return NULL;
	else if (curr->left == NULL)
		return curr;
	else
		return findMin(curr->left);
}

int deleteTree(treePosition curr) {

	if (curr == NULL) return EXIT_SUCCESS;

	deleteTree(curr->left);
	deleteTree(curr->right);
	free(curr);

	return EXIT_SUCCESS;
}