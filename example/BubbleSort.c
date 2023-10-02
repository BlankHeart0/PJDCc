int array[10]={9,5,2,8,1,4,3,0,7,6};

void swap(int*a,int* b)
{
  int temp=*a;
  *a=*b;
  *b=temp;
}

void Bubble_Sort()
{
  int i;
  int j;
  int temp;

  for(i=0;i<10-1;i++)
  {
    for(j=0;j<10-1-i;j++)
    {
      if(array[j]>array[j+1])swap(&array[j],&array[j+1]);
    }
  }

}

int main()
{
  Bubble_Sort();

  int i;
  for(i=0;i<10;i++)
  {
    printf("%d\n",array[i]);
  }

  return 0;
}