#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
    int n,*arr,sum=0;
    scanf("%d",&n);
    arr=(int *)malloc(n*(sizeof(int)));
    int i;
    for(i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }
    for(i=0;i<n;i++)
    {
        sum+=arr[i];
        printf("%d\n", sum);
    }
    printf("%d\n",sum);



    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    return 0;
}

