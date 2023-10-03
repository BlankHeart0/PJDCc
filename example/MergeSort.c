int array[10]={9,5,2,8,1,4,3,0,7,6};
int temp[10];

void MergeSort(int left, int right)
{
	if (left >= right)
	{
		return;
	}

	int mid = (left + right) / 2;

	MergeSort(left, mid);		
	MergeSort(mid + 1, right);	

	int begin1 = left;
    int end1 = mid;			
	int begin2 = mid + 1;
    int end2 = right;		
	int i = begin1;

	while (begin1 <= end1 && begin2 <= end2)
	{
		if (array[begin1] <= array[begin2])
		{
			temp[i] = array[begin1];
			begin1++;
		}
		else
		{
			temp[i] = array[begin2];
			begin2++;
		}
		i++;
	}

	while (begin1 <= end1)
	{
		temp[i] = array[begin1];
		begin1++;
		i++;
	}

	while (begin2 <= end2)
	{
		temp[i] = array[begin2];
		begin2++;
		i++;
	}

    i=left;
    while( i <= right)
    {
        array[i] = temp[i];
        i++;
    }

}

int main()
{
  MergeSort(0,9);
  
  int i;

  for(i=0;i<10;i++)
  {
    printf("%d ",array[i]);
  }

  printf("\n"); 

  return 0;
}