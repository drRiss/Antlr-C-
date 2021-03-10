

int fibbonacci(int n) {
   if(n <= 0){
      return 0;
   }
   if(n == 1){
       return 1;
   }
    else {
      return (fibbonacci(n-1) + fibbonacci(n-2));
   }
}


    int n = 5;
    int i = 0;
	int fib;
    printf("fibonacci del numero:");
   printf("%d" , n);
	
    while(i < n){
      fib = fibbonacci(i);
      printf("%d ",fib);   
      i = i + 1;            
   }