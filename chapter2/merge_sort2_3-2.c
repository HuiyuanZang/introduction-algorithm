#include <stdlib.h>
#include <stdbool.h>

static bool merge_without_sentinels(int *array, unsigned q, unsigned p, unsigned r)
{
    int i, j;
    int *array_sub1, *array_sub2;
    int index1, index2;
    bool ret=true;
    if ( q>p || p>= r){
       ret = false;
       goto ret;
    }
    i = p-q+1;
    j = r-p;//
    array_sub1 = (int*)calloc(i,sizeof(int));
    if( !array_sub1 ){
       ret = false;
       goto ret;
    }
    array_sub2 = (int*)calloc(j,sizeof(int));
    if ( !array_sub2 ){
        ret = false;
        goto err1;
    }
    for ( int k = 0; k<i; k++)
        *(array_sub1 + k) = array[q+k];
    for ( int k = 0; k<j; k++)
        *(array_sub2 + k) = array[p+1+k]; 
    index1=index2=0;
    for (int k = 0; k<r-q+1; k++){
       if( index1 == i-1 && index2 < j-1 ){
          array[p+k] = array_sub2[index2];
          index2++;
          continue;
       }
       if (index2 == j-1 && index1 < i-1){
          array[p+k] = array_sub1[index1];
          index1++;
          continue;
       }
       if ( array_sub1[index1] < array_sub2[index2] ){
            array[p+k] = array_sub1[index1];
            index1++;         
       }else{
           array[p+k] = array_sub2[index2];
           index2++;
       }
    }
err2:
   free(array_sub2);
err1:
   free(array_sub1);
ret:
   return ret;     
}

static void __merge_sort(int * array, unsigned  start, unsigned end)
{
    if ( start < end ) {
        unsigned middle = (start + end)>>1;
        __merge_sort(array,start,middle);
        __merge_sort(array,middle+1,end);
        merge_without_sentinels(array,start,middle,end);
    }
}

void merge_sort_without_sentinels(int *array,unsigned int size)
{
   __merge_sort(array,0,size-1);
}
