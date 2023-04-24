int *pi, **ppi;
int ai[10], *api[10];
int (*pai)[10], *(*papi)[10];
int fi(), (*pfi)(), *(*pfpi)();

int (*signal(int num, int (*handler)()))()
{
    (*pfi)((*pai)[10]);
}
