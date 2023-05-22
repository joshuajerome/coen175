int printf();

int foo(int x, int *y)
{
    int a[10], i, *p;
    char d, *s;


    p = &d;			/* invalid operands to binary = */
    a = p;			/* lvalue required in expression */
    p = (int *) &d;
    p = &a[0];
    p = &i;
    x = 0;

    i = d;
    d = i;

    a[1] = i;
    a[p] = i;			/* invalid operands to binary [] */

    x(1);			/* called object is not a function */
    printf("hello world\n");
    s = &d;

    *i = 0;			/* invalid operand to unary * */
    *a = 0;
    p = p + 1;

    p = (int [20]) a;		/* invalid type in cast expression */
    i = (p < foo);		/* invalid operands to binary < */

    &x = p;			/* lvalue required in expression */

    foo(d, a);
    foo(1, 2, 3);		/* invalid arguments to called function */

    sizeof foo;			/* invalid operand to unary sizeof */
}
