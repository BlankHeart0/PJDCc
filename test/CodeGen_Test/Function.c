// void main()
// {
//   int i; char j;

//   j= 20; print j;
//   i= 10; print i;

//   char k=i-j;
  
//   print k;
  
//   print i*j;
//   i=1;
//   while(i<=5)
//   {
//     print i;
//     i=i+1;
//   }

//   j=253;
//   while(j!=2)
//   {
//     print j;
//     j=j+1;
//   }

// }

long fred()
{
  int a;
  char b;
  a=10;
  b=10;
  return a+b;
}

int main()
{
  int result;
  printint(10);
  result=fred(15);
  printint(result);
  printint(fred(15)+10);
  
  return 1;
}