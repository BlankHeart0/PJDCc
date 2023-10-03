int fib(int x)
{
  if(x==1||x==2)return 1;

  return fib(x-1)+fib(x-2);
}


int main()
{

  int i;
  for(i=1;i<=10;i++)
  { 
    printf("fib(%d)=%d\n",i,fib(i));
  }

  return 0;
}