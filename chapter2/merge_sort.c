#include <stdlib.h>
#include <stdbool.h>
#define max(a,b)  ({             \
     const typeof(a) _a = (a);   \
     const typeof(b) _b = (b);   \
     (void) (&_a == &_b);        \
     _a > _b ? _a:_b;            \
})
static bool merge1(int *array, unsigned q, unsigned p, unsigned r )
{
    int i,j;
    int length1, length2;
    int *sub_array1, *sub_array2;
    if (q>p || p >= r)
        return false;
    length1 = p-q+1;
    length2 = r-p;
    sub_array1 = (int *)calloc(length1+1,sizeof(int));
    sub_array2 = (int *)calloc(length2+1,sizeof(int));
    if ( !sub_array1 || !sub_array2 )
        return false;
    for ( i = 0; i<length1; i++ )
        sub_array1[i] = array[q+i];
    for ( j = 0; j<length2; j++ )
        sub_array2[j] = array[p+1+j];
    sub_array1[length1] = sub_array2[length2] = max(array[p],array[r])+1;
    i = j = 0;
    for ( int k=q; k<=r; k++ ){
        if ( sub_array1[i] <= sub_array2[j] ) {
            array[k] = sub_array1[i];
            i++;   
        } else {
            array[k] = sub_array2[j];
            j++;
        } 
    }
    free(sub_array1);
    free(sub_array2);
    return true;
}
static void  __merge_sort(int *array, unsigned start, unsigned end)
{
    if ( start < end ) {
        unsigned middle = (start + end)>>1;
        __merge_sort(array,start, middle);
        __merge_sort(array,middle+1,end);
        merge1(array,start,middle,end);   
    }
}
void merge_sort(int *array, unsigned int size)
{
   __merge_sort(array,0,size-1); 
}
