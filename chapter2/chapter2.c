#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
/**
 *
 * MACRO
 */

#ifdef __DEBUG__
#define LOG(...)  printf(" %s %s %d:",__FILE__,__FUNCTION__,__LINE__); \
                  printf( __VA_ARGS__)

#define DBG(...)  printf(__VA_ARGS__)
#else
#define LOG(...)
#define DBG(...)
#endif 


#define IsDigit(a) ((((a) >= '0') && ((a) <= '9'))? 1:0)
#define IsMinus(a) (((a) == '-')?1:0)
#define IsPlus(a)  (((a) == '+')?1:0)
#define IsDelimiter(a) (((a) == ' '|| (a) == ';' || (a) == ',')?1:0)


#define BINARY_MASK      0x01
#define BINARY_UNMASK    0xFE
#define CARRY_BIT       (0x01<<1)
#define BINARY_BIT      (0x01)


/*
 * Private structure
 */

static struct option long_options[] = {
    {"sort",required_argument,0,'s' },
    {"add_barray",required_argument,0,'a' },
    {0,0,0,0}
};


/**
 * Private type 
 */

typedef int (*compare_op)(int a, int b);
typedef unsigned char u8;


/**
 *Private functions' declaration
 *
 */

//It is allowed to arrayOut=arrayIn
static void insert_sort(const int *arrayIn, int *arrayOut,size_t  size, compare_op cmp );
static void select_sort(const int *arrayIn, int *arrayOut,size_t size, compare_op cmp );
static void __merge(int *array,int p, size_t size,compare_op cmp);
static void __merge_sort(int *array, size_t size, compare_op cmp);
static void merge_sort(const int *arrayIn, int *arrayOut, size_t size, compare_op cmp);
static void add_barray(const u8 *a, const u8 *b,size_t inSize, u8 *outcome,size_t outSize);
static void array_log(int *array, int size);
static void barray_log (u8 *array, size_t size);
static int default_cmp(int a, int b);
/**
 *Private functions' defination
 */


static void insert_sort(const int *arrayIn, int *arrayOut,size_t size, compare_op cmp )
{
    int i, j, cur;
    if (!arrayIn || !arrayOut || size == 0 || !cmp)
    {
        LOG("Wrong arugument arrayIn=%p arrayOut=%p size=%zd cmp=%p", arrayIn,arrayOut,size,cmp);
        return;
    }
    arrayOut[0] = arrayIn[0];
    for (i=1;i<size;i++)
    {
        cur = arrayIn[i];
        for (j=i-1; j>=0 && cmp(cur,arrayOut[j])>0; j--)
        {
             arrayOut[j+1] = arrayOut[j];
        }
        arrayOut[j+1] = cur;
    }
}

static void select_sort(const int *arrayIn, int *arrayOut,size_t size, compare_op cmp )
{
    int i,j;
	int min, minIndex;
    memcpy(arrayOut,arrayIn, sizeof(int)*size);
    for(i=0;i<size-1;i++)
	{
	    min = arrayOut[i];
		minIndex = i;
	    for(j=i+1;j<size;j++)
		{
		    if( cmp(arrayOut[j], min)>0 )
			{
			    min = arrayOut[j];
				minIndex = j;
			}
		}
		arrayOut[minIndex] = arrayOut[i];
		arrayOut[i] = min;
	}
}

//To merge sorted array[0-p](p+1)  and sorted array[p+1--size-1] (size-p-1)
//then become sorted array'[0--size-1]
static void __merge(int *array,int p, size_t size,compare_op cmp)
{
    int i, j;
    if ( !array )
    {
        fprintf(stderr, "argument is NULL");
        return;
    }
    if ( p > size-1  )
    {
        fprintf(stderr, "th");
        return;
    }

    for (i = p+1; i < size; i++)
    {
        int cur = array[i];
        for (j = i-1; j >= 0&&cmp(array[j],cur)<0; j--)
        {   
            array[j+1] = array[j];
        }
        array[j+1] = cur;
    }

}

static void __merge_sort(int *array, size_t size, compare_op cmp)
{
    if (!array)
    {
        fprintf(stderr,"array is NULL\n");
    }
    }
    if (size == 0)
    {
        fprintf(stderr, "Empty array\n");
        return;
    }
    //the recur
    if (size == 1)
        return;
    __merge_sort(array,(size)>>1,cmp);
    __merge_sort(&array[(size>>1)],size-(size>>1), cmp);
    __merge(array,(size>>1)-1,size,cmp);
}

static void merge_sort(const int *arrayIn, int *arrayOut, size_t size, compare_op cmp)
{
    memcpy(arrayOut,arrayIn,sizeof(int)*size);
    __merge_sort(arrayOut,size,cmp);
}
static void add_barray(const u8 *a, const u8 *b,size_t inSize, u8 *outcome,size_t outSize)
{
    int i;
    u8 carry = 0x00;
    if (!a || !b || !outcome)
    {
        fprintf(stderr, "argument is NULL a=%p b=%p outcome=%p\n",a,b,outcome);
    }
    if (outSize < inSize+1 )
    {
        fprintf(stderr, "outSize should be inSize + 1\n");
    }
    for (i=0; i< inSize; i++)
    {
        outcome[i]= (a[i]&BINARY_MASK)^(b[i]&BINARY_MASK)^(carry&BINARY_MASK);
        // Or set carry based on checking if (a[i] + b[i] + carry) >= 2
        carry =((a[i]&BINARY_MASK) +(b[i]&BINARY_MASK) + carry) > 1?0x01:0x00;
    #if 0
        if ( !outcome[i] && (!(a[i]&BINARY_MASK)||!(b[i]&BINARY_MASK)||!(carry&BINARY_MASK)))
            carry = 0x01;
	    else if (outcome[i]&BINARY_MASK && a[i]&BINARY_MASK && b[i]&BINARY_MASK && carry&BINARY_MASK )
	        carry = 0x01;
		else
		    carry = 0x00;
    #endif 
    }
    outcome[inSize] = carry;
}

static void array_log(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        DBG(" array[%d]=%d ",i, array[i]);
        if( i%16 == 0 && i != 0 )
            DBG("\n");
    }
    DBG("\n");
}
static void barray_log(u8 *array, size_t size)
{
    DBG("array={ ");
    for (int i = 0; i<size;i++)
	{
        DBG("0x%02x,", array[i]);
    }
    DBG("}\n");
}
static int default_cmp(int a, int b)
{
    return a - b;
}
/**
 *Public functions' defination
 */

int main (int argc, char *argv[])
{
    int opt;
    int i;
    int optionIndex = 0;
    int *arrayIn = NULL;
    int arraySize = 0;
    //INT_MAX = 2147483647, INT_MIN = -2147483648
    //10 bytes + 1 byte symbol char is enough for depicting integer from [INT_MIN,INT_MAX]
    char intValue[11]; 

    while ((opt = getopt_long(argc,argv,"s:a",long_options, &optionIndex )) != -1)
    {
        int newValueSize = 0;
        int arrayBufferSize = 64;
		int *arrayOut = NULL;
        switch (opt)
        {
        case 's':
            printf("optarg[%zd]=%s \n",strlen(optarg),optarg);
            arrayIn = (int *)malloc(sizeof(int)*arrayBufferSize);
            if(!arrayIn)
            {
                fprintf(stderr,"Cannot allocate memory for arrayIn\n");
                exit(1);
            }

            //Parsing optarg
            for (i = 0; i < strlen(optarg); i++)
            {
                if ( IsMinus(optarg[i]) || IsPlus(optarg[i]))
                {
                    intValue[10] = optarg[i];
                }
                if ( IsDigit(optarg[i]))
                {
                    if (newValueSize >= 10)
                    {
                        fprintf(stderr, "the value input should between [%d, %d]\n", INT_MIN, INT_MAX);
                        free(arrayIn);
						exit(1);
                    }
                    intValue[newValueSize++] = optarg[i];
                }
                if (IsDelimiter(optarg[i]) || i == strlen(optarg)-1 )
                {
                    int j;
                    int value = 0;
                    for (j = 0; j < newValueSize; j++ )
                    {
                        value += (intValue[j]-'0')*(int)(pow((double)10,(double)(newValueSize-1-j)));
                    }
                    value = IsMinus(intValue[10])?((-1)*value):value;
                    //generate arrayIn and append to array
                    if ( newValueSize != 0 )
                    {
                        arrayIn[arraySize++] = value;
                    }
                    if (arraySize >= arrayBufferSize)
                    {
                        arrayBufferSize <<= 1;
                        arrayIn = (int *)realloc(arrayIn,sizeof(int)*arrayBufferSize);
                        if(!arrayIn)
                        {
                            fprintf(stderr,"Cannot reallocate memory for arrayIn\n");
                            free(arrayIn);
							exit(1);
                        }
                    }
                    newValueSize = 0;
                    intValue[10] = '\0';
                }
            }
			arrayOut = (int *) malloc(sizeof(int)*arraySize);
			if(!arrayOut)
			{
			    fprintf(stderr,"Cannot allocate memory for arrayOut\n");
                free(arrayIn);
				exit(1);
			}
			//insert_sort 
			LOG("insert_sort\n");
            array_log(arrayIn,arraySize);
            insert_sort(arrayIn,arrayOut,arraySize,default_cmp);
            array_log(arrayOut,arraySize);
			memset(arrayOut,0x00,arraySize);
			//select_sort 
	        LOG("select_sort\n");
            array_log(arrayIn,arraySize);
            select_sort(arrayIn,arrayOut,arraySize,default_cmp);
	        array_log(arrayOut,arraySize);
			memset(arrayOut,0x00,arraySize);

			//merge_sort 
	        LOG("merge_sort\n");
            array_log(arrayIn,arraySize);
            merge_sort(arrayIn,arrayOut,arraySize,default_cmp);
	        array_log(arrayOut,arraySize);


			if(arrayIn)
			    free(arrayIn);
			if(arrayOut)
			    free(arrayOut);
            break;
        case 'a':
            {
            const static u8 a[] = {0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x00};
            const static u8 b[] = {0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01};
			// a+b =              {0x00,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x00,0x01)
            u8 out[32];
            memset(out,0x00,sizeof(out));
            add_barray(a,b,sizeof(a),out,sizeof(out));
            barray_log(out,sizeof(a)+1);
            }
            break;
        default:
            printf("Usage: %s [-i \"90 78 -60 45 79 86 789\" ] \n",argv[0]);
            break;
        case '?':
            printf("Usage: %s [-i \"90 78 -60 45 79 86 789\" ] \n",argv[0]);
            break;
        }
    }
    exit(0);

}
