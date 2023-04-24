/* The most epic simple C test file ever */

int a(), *b(), **c();
char _d(), *e_f(), *(*g_)();

/* Test cases from SS8 */

int x;
char *p;
char a[10];
int **q;
char *b[10];
char (*c)[10];
int **d[10];
int *(*e)[10];

int f();
char *g();
char a()[10];
int b[10]();
int **h();
char (*p)();
int *(*q)();


/* Jashan's Test Cases */

int main();

int main(int number) {}

int main(int number, int z, int x) {}

int main(void) {
    int number;

    printf("Enter an integer: ");
    scanf("%d", &number);

    if (number < 0) {
        printf("You entered %d.\n", number);
    }

    printf("The if statement is easy.");

    return 0;
}

/* Josh's Test Cases */

int test_comparisons(int x, char y) {

	/* Also check break, return */

	if ((y < x) && (y <= x) || (y > x) && (y >= x)) {
		x = x + 1;
	}
	return 0;
}

char ***_testing_loops_and_statements_(int (*func)(), char c, int x) {

	
	int a,b,c;
	
	/* Nested while loops */
	
	while (x < 50) {
		x = x + 1;
		if (x >= 10) {
			while (x < 30) {
				x = x + 2;
				break;
			}
		}
	}

	/* Nested for loops */

	for (i = 0; i < n; i = i + 1) {
		for (j = 0; j < i; j = j + 1) {
			printf("this does nothing");
		}
	}

	/* Funny for loops */

	for (i = 0; i < n; i = i + 1) 
		for (j = 0; j < i; j = j + 1)
			if (raghav == gay)
				break;
			else
				raghav = notGay;


	/* loops in loops in statements in loops in statements */

	if (youWantTorture) {

		while (single) {
		
			for (age = 0; age < death; age = age + 1) {
				
				if (woman == found) {
					marry_her();
				} else {
					look_for_woman();
				}
			}
			
			if (super_down_bad == 1) {
				die();
			}
		}
	}


	return some_random_return_type;

}

/* Raghav's Test Cases */

int p(void) {
    while (i < 5) {
        i = i + 1;
        i = i +3;
        if (i == 4) {
            break;
        }
    }
    for (i = 0; i < 10; i=i+1) {
        i = i +2;
        if ( i == 4) {
            return 3;
        }
        else {
            return 1 == 3;
        }
    }
}

int x(void) {
    if (a == b) x = 4; else x = 69;
    if (a <= b) {
        x = 3;
        if (x == 4) {
            x = 3;
        } else {
            x = 420;
        }
    } else {
        if (s == 234) {
            if (x == 32423) {
                x = 5; 
            }
            else {
                return (x % 5 + 3 - 4 / 2 * 8 - 69 + 420 / 190);
            }
        }
        x = 4;
    }
}
