/*
 * tree2.c, or is it any better with function pointers?
 */

int printf(), *malloc(), *null;

int **insert(int **root, int *data, int cmp())
{
    if (!root) {
	root = (int **) malloc(sizeof(*root) * 3);
	root[0] = data;
	root[1] = null;
	root[2] = null;
    } else if (cmp(data, root[0]) < 0) {
	root[1] = (int *) insert((int **) root[1], data, cmp);
    } else if (cmp(data, root[0]) > 0)
	root[2] = (int *) insert((int **) root[2], data, cmp);

    return root;
}

int search(int **root, int *data, int (*cmp)())
{
    int diff;


    if (!root)
	return 0;

    diff = (*cmp)(data, root[0]);

    if (diff < 0)
	return search((int **) root[1], data, cmp);

    if (diff > 0)
	return search((int **) root[2], data, cmp);

    return 1;
}

int preorder(int **root)
{
    if (root) {
	printf("%d\n", *root[0]);
	preorder((int **) root[1]);
	preorder((int **) root[2]);
    }
}

int inorder(int **root)
{
    if (root) {
	inorder((int **) root[1]);
	printf("%d\n", *root[0]);
	inorder((int **) root[2]);
    }
}

int compare(int *x, int *y)
{
    return *x - *y;
}

int main(void)
{
    int **root;
    int a[10], i;

    i = 0;

    while (i < 8) {
	a[i] = i;
	i = i + 1;
    }

    root = (int **) null;
    root = insert(root, &a[7], &compare);
    root = insert(root, &a[4], &compare);
    root = insert(root, &a[1], &compare);
    root = insert(root, &a[0], &compare);
    root = insert(root, &a[5], compare);
    root = insert(root, &a[2], compare);
    root = insert(root, &a[3], compare);
    root = insert(root, &a[6], compare);
    printf("preorder traversal:\n");
    preorder(root);
    printf("inorder traversal:\n");
    inorder(root);
}
