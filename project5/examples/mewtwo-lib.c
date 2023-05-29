/*
 * This file will not be run through your compiler.
 */

# include <stdio.h>

extern int f();
extern int b;

int main(void)
{
    printf("%d\n", b);
    f(4);
}