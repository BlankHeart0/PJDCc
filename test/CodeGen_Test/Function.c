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
  int a;
  char b;
long fred()
{

  a=10;
  b=10;
  return a+b;
}
int result;
int main()
{
  
  print_int(10);
  result=fred(15);
  print_int(result);
  print_int(fred(15)+10);
  
  return 1;
}