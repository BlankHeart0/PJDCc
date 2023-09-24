
int array[10];

//{9,5,2,8,1,4,3,0,7,6}
//{6,5,2,8,1,4,3,0,7,9}
//{3,5,2,0,1,4,6,7,8,9}
//{0,1,2,3,4,5,6,7,8,9}


void Init()
{
  array[0]=9;array[1]=5;array[2]=2;array[3]=8;array[4]=1;
  array[5]=4;array[6]=3;array[7]=0;array[8]=7;array[9]=6;
}

void Quick_Sort(long begin,long end)
{
  // print_int(begin);
  // print_int(end);
  // print_int(111);    


  // if(++time==6)return;

    if(begin>=end)return;

    int left=begin;
    int right=end;

    int key_i=begin;
    int temp;
    
    while(begin<end)
    {
        while(array[end]>=array[key_i]&&begin<end)end--;
        while(array[begin]<=array[key_i]&&begin<end)begin++;

        temp=array[begin];array[begin]=array[end];array[end]=temp;
    }

    temp=array[key_i];array[key_i]=array[end];array[end]=temp;
    key_i=end;

    Quick_Sort(left,key_i-1);
    Quick_Sort(key_i+1,right);
}

int main()
{
  Init();
  Quick_Sort(0,9);

  int i=0;
  while(i<10)
  {
    print_int(array[i]);
    i=i+1;
  }

  return 0;
}