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