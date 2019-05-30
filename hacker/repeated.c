#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void main(){


	// Used as index in the modified string 
	int index = 0;
	char str[100];
	scanf("%s",str);    
	int n=strlen(str)+1;
	// Traverse through all characters 
	for (int i=0; i<n; i++) { 

		// Check if str[i] is present before it   
		int j;   
		for (j=0; j<i; j++)  
			if (str[i] == str[j]) 
			{// printf("i=%d ,j=%d , %c",i,j,str[i]);
				break; 
			}

		// If not present, then add it to 
		// result. 
		if (j == i) 
		{
			str[index++] = str[i]; 
		//	printf("i=%d ,j=%d , %c",i,j,str[i]);
		}
	} 

	printf("%s",str); 


}

