int array[10]={9,5,2,8,1,4,3,0,7,6};

void swap(int*a,int* b)
{
  int temp=*a;
  *a=*b;
  *b=temp;
}

void Quick_Sort(int begin,int end)
{
  if(begin>=end)return;
  int left=begin;
  int right=end;
  int key_i=begin;
  int temp;
    
  while(begin<end)
  {
    while(array[end]>=array[key_i]&&begin<end)end--;
    while(array[begin]<=array[key_i]&&begin<end)begin++;
    swap(&array[begin],&array[end]);
  }

  temp=array[key_i];array[key_i]=array[end];array[end]=temp;
  key_i=end;

  Quick_Sort(left,key_i-1);
  Quick_Sort(key_i+1,right);
}

int main()
{
  Quick_Sort(0,9);

  int i;
  for(i=0;i<10;i++)
  {
    printf("%d ",array[i]);
  }

  printf("\n");

  return 0;
}