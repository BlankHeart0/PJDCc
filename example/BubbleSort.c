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

  int done_flag;

  for(i=0;i<10-1;i++)
  {
    done_flag=1;

    for(j=0;j<10-1-i;j++)
    {
      if(array[j]>array[j+1])
      {
        swap(&array[j],&array[j+1]);
        done_flag=0;
      }
    }

    if(done_flag)break;
  }

}

int main()
{
  Bubble_Sort();

  int i;
  for(i=0;i<10;i++)
  {
    printf("%d ",array[i]);
  }

  printf("\n");

  return 0;
}