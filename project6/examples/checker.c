int x, y, z;

int foo(void)
{
    /* check assignment */
    x = 5;
    y = 5;
    y = x;
    z = y;

    /* check add */
    x + y;
    y + z;
    z + x;

    x = y + z;
    y = x + z;
    z = x + y;

    /* check sub */
    y - x;
    z - y;
    x - z;

    x = y - z;
    y = x - z;
    z = x - y;

    /* check mul */
    y * x;
    z * y;
    x * z;

    x = y * z;
    y = x * z;
    z = x * y;

    /* check div */
    y / x;
    z / y;
    x / z;

    x = y / z;
    y = x / z;
    z = x / y;

    /* check rem */
    y % x;
    z % y;
    x % z;

    x = y % z;
    y = x % z;
    z = x % y;

    /* check relational */
    x < y;
    x > y;
    x <= y;
    x >= y;
    x == y;
    x != y;

    /* check unary */
    !x;
    !x;
    -x;
    -x;

    return 0;
}