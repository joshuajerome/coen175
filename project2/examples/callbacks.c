int (*f)();

int eval(int (*g)(), int x)
{
    return g(x);
}

int incr(int x)
{
    return x + 1;
}

int main(void)
{
    int (*g)();

    g = incr;
    eval(g, 0);
}
