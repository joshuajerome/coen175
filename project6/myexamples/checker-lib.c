# include <stdio.h>

int print(int x)
{
    printf("param = %d\n", x);
}

int printError(void)
{
    printf("ERROR\n");
}

int debug(int x)
{
    printf("DEBUG: %d\n", x);
}

int printAll(int x, int y, int z)
{
    printf("x = %d\ty = %d\tz = %d\n", x, y, z);
}

int printBool(int x)
{
    debug(x);
    if (x == 1)
        printf("true\n");
    else if (x == 0)
        printf("false\n");
    else printError();
}