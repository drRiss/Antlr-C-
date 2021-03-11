 
	int dividerCount(int from, int to, int count){
		    while(from < to){
      			if(to % from == 0){
      				count = count + 1;
				}
				from = from + 1;
			}
			return count;
	}

    int Number = 10;
    int i = 1;
    int j;
    int Count; 

 	while (i <= Number){
   		Count = 0;
    	if(Number % i == 0){
      		j = 1;
			Count = dividerCount(j, i, Count);
			if(Count == 2){
				printf("%d", i);
			} 
      	}
    	i = i + 1;
   	}

