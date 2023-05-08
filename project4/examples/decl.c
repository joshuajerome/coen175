int f1()[10];			/* 'f1' has invalid type */
int (*f2())[10];
int (*f3())[10][20];

int a1[10]();			/* 'a1' has invalid type */
int (*a2[10])();
int (**a3[10])();

int a[10][20];
