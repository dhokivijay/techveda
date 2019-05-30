#include<stdio.h>
#include<stdlib.h>
void main(){


int sum,arr_columns=6,arr_rows=6;
int **arr;


sum=hourglassSum(int arr_rows, int arr_columns, int** arr) {
printf("%d\n",sum);






}





int hourglassSum(int arr_rows, int arr_columns, int** arr) {
    int R=arr_rows,C=arr_columns,max_sum=0;
  for (int i = 0; i < R - 2; i++) {
    for (int j = 0; j < C - 2; j++) {
      // Considering mat[i][j] as top left cell of
      // hour glass.
      int sum = (arr[i][j] + arr[i][j + 1] + arr[i][j + 2]) +
                (arr[i + 1][j + 1]) +
                (arr[i + 2][j] + arr[i + 2][j + 1] + arr[i + 2][j + 2]);

      // If previous sum is less then current sum then
      // update new sum in max_sum
      if(sum>=max_sum)
      max_sum =sum;
    }
  }
  return max_sum;
}
