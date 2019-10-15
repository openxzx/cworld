#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
        int data;

        struct TreeNode* lchild;
        struct TreeNode* rchild;
} TreeNode;

int create_tree(TreeNode *T)
{
	int data;

	while (1) {
		scanf("%d", &data);
		if (data < 0)
			continue;
		else
			break;
	}

	T = (TreeNode *)malloc(sizeof(TreeNode));
	T->data = data;
	create_tree(T->lchild);
	create_tree(T->rchild);

	return 0;
}

int main(int argc, char *argv[])
{
	int n = 0;

	if (argc > 2)
		printf("Input parameters error.");
	else {
		n = *argv[1] % 48;
		printf("Tree count: %d\n", n);
	}

	//TreeNode *T;
}
