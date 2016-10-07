#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#include "sort.h"
static int random_number(int min, int max)
{
   int ret=0;
   if( min < max )
   {
       max+=1;
   }
   else
   {
      int tmp = max;
      max = min +1;
      min = tmp;
   }
  // srand(time(NULL));
  // printf("rand()=%d\n",rand());
   ret = (rand()%(max-min)) + min;
   return ret;
} 
static void pr_times(clock_t real,struct tms *tmsstart, struct tms *tmsend) 
{
    static long clktck = 0;
	if (clktck == 0)
	{ 
		if ((clktck = sysconf(_SC_CLK_TCK)) <0 )
		{
			fprintf(stderr,"sysconf error");
			exit(EXIT_FAILURE);
		}
	}
	printf("    real:   %7.7f\n",real/(double)clktck);
	printf("    user:   %7.7f\n",(tmsend->tms_utime - tmsstart->tms_utime)/(double)clktck);
	printf("    sys:    %7.7f\n",(tmsend->tms_stime - tmsstart->tms_stime)/(double)clktck);
	printf("   child user:   %7.7f\n",(tmsend->tms_cutime - tmsstart->tms_cutime)/(double)clktck);
	printf("   child sys:   %7.7f\n",(tmsend->tms_cstime - tmsstart->tms_cstime)/(double)clktck);
//	printf("    real:   %Lf\n",(long double)real);
//	printf("    user:   %Lf\n",(long double)(tmsend->tms_utime - tmsstart->tms_utime));
//	printf("    sys:    %Lf\n",(long double)(tmsend->tms_stime - tmsstart->tms_stime));
//	printf("   child user:   %Lf\n",(long double)(tmsend->tms_cutime - tmsstart->tms_cutime));
//	printf("   child sys:   %Lf\n",(long double)(tmsend->tms_cstime - tmsstart->tms_cstime));
}


typedef void (*action_f)(int*,unsigned);
typedef struct{
	char* name;
	char* alise;
	action_f action;
}cmd_table_t;

static cmd_table_t cmd_table[]={
	{"insert","i", insert_sort},
	{"insert2","i2",insert_sort2},
	{"selection","s",selection_sort},
	{"merge","m",merge_sort},
        {"merge2","m2",merge_sort_without_sentinels}
};


static void do_function(action_f cmd, int *array, unsigned size)
{
    struct tms tmsstart, tmsend;
	clock_t start, end;
	if ((start = times(&tmsstart)) == -1)
	{
		fprintf(stderr,"times error!");
		exit(EXIT_FAILURE);
	}
    cmd(array, size);
	if ((end = times(&tmsend)) == -1)
	{
		fprintf(stderr,"times error!");
		exit(EXIT_FAILURE);
	}
    pr_times(end-start,&tmsstart,&tmsend);

}
static void err_exit(char* name)
{
    fprintf(stderr,"usage: %s [-n size(>0)] [-m min] [-M max(>min)] [-o (i)nsert|(i2)nsert2|(s)election|(m)erge]",name);
    exit(EXIT_FAILURE);
}
void main (int argc, char* argv[])
{   
    int opt;
    int count,min,max;
    int *array;
	char *operation = NULL;
	count=min=max=0;
    srand(time(NULL));
	while(( opt = getopt(argc,argv,"-n:-m:-M:-o:")) != -1 )
    {
       switch(opt)
       {
          case 'n':
             count = atoi(optarg);
          break;
          case 'm':
             min = atoi(optarg);
          break;
          case 'M':
             max = atoi(optarg);
          break;
		  case 'o':
		     operation = optarg;
		  break;
          default:
             err_exit(argv[0]);
		  break;
       }
    }
    if ( count <= 0 ||  max<= min )
    {
		err_exit(argv[0]);
    }
	printf("size=%d;min=%d;max=%d;operation=%s \n",count,min,max, operation);
    array = (int *)calloc(count, sizeof(int));
    for (int i=0; i< count; i++)
    {
        array[i] = random_number(min,max);
        printf("array[%d]=%d\n",i,array[i]);
    }
	//printf("the number of cmd_table is %d \n",sizeof(cmd_table)/sizeof(cmd_table_t) );
	for(int i=0;i<sizeof(cmd_table)/sizeof(cmd_table_t);i++)
	{
		if ( strcmp(cmd_table[i].name,operation) == 0 ||
			 strcmp(cmd_table[i].alise,operation) == 0 )
		{
               printf("%s sort \n",operation);
	           do_function(cmd_table[i].action,array,count);
               printf("after sort\n");
			   for (int i=0; i<count; i++)
               {
                   printf("array[%d]=%d\n",i,array[i]);
               }
			   free(array);
			   exit(0);
		}
	}
	free(array);
    err_exit(argv[0]);    
}
