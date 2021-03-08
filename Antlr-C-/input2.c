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


 
 
    int Number = 10;
    int i = 1;
    int j;
    int Count; 
 
 	while (i <= Number)
   	{
   		Count = 0;
    	if(Number % i == 0)
      	{
      		j = 1;
      		while(j < i)
      		{
      			if(i % j == 0)
      			{
      				Count = Count + 1;
				}
				j = j + 1;
			}
			if(Count == 2)
			{
				printf(i);
			} 
      	}
    	i = i + 1;
   	}