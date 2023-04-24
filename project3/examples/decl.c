int (*a)[10];
int (*a)[10];
int *a[10];				/* conflicting types for 'a' */

int b[10][20];
int b[20][10];				/* conflicting types for 'b' */

int *c();
int *c();
int (*c)();				/* conflicting types for 'c' */

int (*(*d[10])())[10];
int (*(*d[10])())[20];			/* conflicting types for 'd' */
