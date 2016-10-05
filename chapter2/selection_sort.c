/*
 *find the smallest [A[0]...A[n-1]] and replace with A[0], then
 *find the smallest [A[1]...A[n-1]] and replace with A[1], then
 *........................................
 *
 *
 *for i=0 to A.length 
 *    smallest = A[i]
 *    index = i
 *    for j=i+1 to A.length
 *        if A[j] < smallest
 *            smallest = A[j]
 *            index = j
 *    A[index]=A[i]
 *    A[i]=smallest 
 *
 */
void selection_sort(int *array, unsigned int size)
{
	for (int i=0;i<size;i++)
	{
		int j=0;
		int smallest = array[i];
		int index = i;
		for (j=i+1; j<size; j++)
		{
			if (array[j] < smallest )
			{
				smallest = array[j];
				index = j;
			}
		}
		array[index] = array[i];
		array[i] = smallest;
	}
}
