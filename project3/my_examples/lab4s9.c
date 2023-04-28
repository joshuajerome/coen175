int **a;
int b[10];
int *c[11];
char **d[12];

int testing_pointer_replacement_on_arrays(int e[13], char *f[14]) {
    return 0;
}

char testing_pointer_replacement_on_func(int g(), char (*h)()) {
    return 'a';
}

int testing_both(int e[13], char *f[14], int g(), char (*h)()) {
    return 0;
}