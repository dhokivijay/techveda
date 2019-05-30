#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {

	char *s;
	int i,j,cnt=0;
	s = malloc(1024 * sizeof(char));
	scanf("%[^\n]", s);
	s = realloc(s, strlen(s) + 1);
	for(i=0;s[i];i++)
	{	printf("%c",s[i]);
		if(s[i]==32)
		{
	//		for(j=cnt;s[i]==32;j++)
	//			printf("%c",s[j]);

			printf("\n");
	//		cnt=0;continue;
		}
	
	}



	//Write your logic to print the tokens of the sentence here.
	return 0;
}


