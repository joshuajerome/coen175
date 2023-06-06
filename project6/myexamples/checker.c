int print();
int printAll();
int printBool();
int printError();
int debug();

int x, y, z;

int assignment(void)
{
    x = 5;
    y = 5;
    y = x;
    z = y;
}

int add(void)
{
    x = y + z;
    y = x + z;
    z = x + y;
}

int sub(void)
{
    x = y - z;
    y = x - z;
    z = x - y;
}

int mul(void)
{
    x = x * 2;
    y = y * 2;
    z = z * 2;
}

int div(void)
{
    x = x / 2;
    y = y / 2;
    z = z / 2;
}

int rem(void)
{
    x = y % z;
    y = x % z;
    z = x % y;
}

int relational(int num, int param1, int param2)
{
    int result;
    
    if (num == 0)
        result = (param1 < param2);
    else if (num == 1)
        result = (param1 > param2);
    else if (num == 2)
        result = (param1 <= param2);
    else if (num == 3)
        result = (param1 >= param2);
    else if (num == 4)
        result = (param1 == param2);
    else if (num == 5)
        result = (param1 != param2);
    else
        result = -1;

    /* print(result); */
    
    return result;
}

int not(int param)
{
    return !param;
}

int negate(int param)
{
    return -param;
}

int main(void)
{
    int count;

    /* check assignment */
    assignment();
    printAll(x, y, z);

    /* check add */
    add();
    printAll(x, y, z);

    /* check sub */
    sub();
    printAll(x, y, z);

    /* check mul */
    mul();
    printAll(x, y, z);

    /* check div */
    div();
    printAll(x, y, z);

    /* check rem */
    rem();
    printAll(x, y, z);

    /* check relational */
    for (count = 0; count < 6; count = count + 1)
    {
        printBool(relational(count, x, y));
    }

    /* check not */
    print(not(x));
    print(not(x));

    /* check negate */
    print(negate(x));
    print(negate(x));

    return 0;
}