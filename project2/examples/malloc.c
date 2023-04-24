/* malloc.c */

int *malloc();

int main(void)
{
    int n;
    int *p;

    n = 10;
    p = (int *) malloc(sizeof(*p) * n);
}
