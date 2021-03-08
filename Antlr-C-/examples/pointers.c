 int* a;
int* b;
int x = 5;
int y = 6;
int temp;

a = &x;
b = &y;

temp = *b;
*b = *a;
*a = temp;

printf("%d", x);
printf("%d", y);


 
