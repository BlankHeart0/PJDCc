// int sum(int a,int b)
// {
//   int c=1;
//   return a+b+c;
// }

// int main()
// {
//   int x=2;
//   int y=1;

//   print_int(x);
//   print_int(y);
//   print_int(sum(x,y));

//   return 0;
// }

int fib(int x)
{
  if(x==1||x==2)return 1;

  return fib(x-1)+fib(x-2);
}

int factorial(int n)
{
  if(!n)return 1;

  return n*factorial(n-1);
}

int main()
{

  int i=1;
  while(i<=10)
  {
    print_int(factorial(i));
    i=i+1;
  }

  return 0;
}