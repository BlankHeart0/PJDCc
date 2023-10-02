int array[10]={9,5,2,8,1,4,3,0,7,6};
int temp[10];

void MergeSort(int left, int right)
{
    //分解：
    //分割数组只有一个元素时停止递归
	if (left >= right)
	{
		return;
	}

	int mid = (left + right) / 2;

	MergeSort(left, mid);		//分割并排序数组左半边
	MergeSort(mid + 1, right);	//分割并排序数组右半边

    //合并：
	int begin1 = left;
    int end1 = mid;			//数组1的左右区间
	int begin2 = mid + 1;
    int end2 = right;		//数组2的左右区间
	int i = begin1;

    //排序两个有序数组
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
  int i=0;
  while(i<10)
  {
    printf("%d\n",array[i]);
    i=i+1;
  }

  return 0;
}