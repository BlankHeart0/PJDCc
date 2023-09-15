int array[10];

void Init()
{
  array[0]=9;array[1]=5;array[2]=2;array[3]=8;array[4]=1;
  array[5]=4;array[6]=3;array[7]=0;array[8]=7;array[9]=6;
}

int i;
int j;
int temp;
void Bubble_Sort()
{
  i=0;
  while(i<10-1)
  {
    j=0;
    while(j<10-1-i)
    {
      if(array[j]>array[j+1])
      {
        temp=array[j];array[j]=array[j+1];array[j+1]=temp;
      }
      j=j+1;
    }
    i=i+1;
  }
}

int main()
{
  Init(0);
  Bubble_Sort(0);

  i=0;
  while(i<10)
  {
    printint(array[i]);
    i=i+1;
  }
  return 0;
}