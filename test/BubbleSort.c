int array[10];

void Init()
{
  array[0]=9;array[1]=5;array[2]=2;array[3]=8;array[4]=1;
  array[5]=4;array[6]=3;array[7]=0;array[8]=7;array[9]=6;
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
      if(array[j]>array[j+1])
      {
        temp=array[j];array[j]=array[j+1];array[j+1]=temp;
      }
    }
  }

}

int main()
{
  Init();
  Bubble_Sort();

  int i;
  for(i=0;i<10;i++)
  {
    printf("%d\n",array[i]);
  }

  return 0;
}