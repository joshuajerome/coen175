int NUM;
int lookahead, expr();
int c, lexval;
int (*ops[256])();

int printf(), exit(), getchar(), isspace(), isdigit();

int add(int x, int y)
{
    return x + y;
}


int sub(int x, int y)
{
    return x - y;
}


int mul(int x, int y)
{
    return x * y;
}


int div(int x, int y)
{
    return x / y;
}

int lexan(void)
{
    int n;


    if (c == 0)
	c = getchar();

    while (isspace(c) && c != '\n')
	c = getchar();

    if (!isdigit(c)) {
	int x;
	x = c;
	c = 0;
	return x;
    }

    n = 0;

    while (isdigit(c)) {
	n = n * 10 + c - 48;
	c = getchar(c);
    }

    lexval = n;
    return NUM;
}


/* Look familiar? */

int match(int token)
{
    if (lookahead != token) {
	printf("syntax error at %d\n", lookahead);
	exit(1);
    }

    lookahead = lexan();
}


int factor(void)
{
    int n;


    if (lookahead == '(') {
	match('(');
	n = expr();
	match(')');
	return n;
    }

    n = lexval;
    match(NUM);
    return n;
}


int term(void)
{
    int n, op;
    
    
    n = factor();

    while (lookahead == '*' || lookahead == '/') {
	op = lookahead;
	match(lookahead);
	n = ops[op](n, factor());
    }

    return n;
}


int expr(void)
{
    int n, op;


    n = term();

    while (lookahead == '+' || lookahead == '-') {
	op = lookahead;
	match(lookahead);
	n = ops[op](n, term());
    }

    return n;
}

int main(void)
{
    int n;
    NUM = 256;


    ops['+'] = add;
    ops['-'] = sub;
    ops['*'] = mul;
    ops['/'] = div;

    lookahead = lexan();

    while (lookahead != -1) {
	n = expr();
	printf("%d\n", n);

	while (lookahead == '\n')
	    match('\n');
    }
}
