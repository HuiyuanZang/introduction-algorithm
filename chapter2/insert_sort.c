void insert_sort(int * array, unsigned int size)
{
	if ( size <=1 )
	  return;
	for (int i = size-2;i>=0;i--)
	{
		//int cur = array[i];
		for (int j=1;j<size-i;j++)
		{
			if ( array[i+j-1]>array[i+j] )
			{
				int tmp = array[i+j-1];
				array[i+j-1] = array[i+j];
				array[i+j] = tmp;
			}
			else
			  break;
		}

	}
}
void insert_sort2(int *array, unsigned int size)
{
	for (int j=1;j<size;j++)
 	{
            int i;
	    int key = array[j];
            for (i=j-1; i>=0&&array[i]>key;i--)
                array[i+1] = array[i];
             array[i+1] = key;
	}
}
