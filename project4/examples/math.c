int x, z;
char y;
char *s, *t;
int *p, *q;

int main(void)
{
    x || y && z;
    x && y || x;

    x == y != z;
    x != y == z;

    x < y > z;
    x > y < z;
    x <= y >= z;
    x >= y <= z;

    x + y - z;
    x - y + z;

    x * y / z;
    x / y % z;
    x % y * z;

    p + x + y;
    x + p + y;

    s + x + y;
    x + s + y;

    p - q + x;
    x + p - q;

    s - p + x;			/* invalid operands to binary - */
    s - t - y;

    - x;
    - y;
    - p;			/* invalid operand to unary - */

    ! x;
    ! y;
    ! x * y;
    ! p * y;

    &p + x;
    &s - x;

    x + sizeof p;
    x * sizeof s;
    x * sizeof sizeof p;
}
