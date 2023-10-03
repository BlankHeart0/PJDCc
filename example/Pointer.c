int   a;
int  *b;

int main()
{
  int  c;
  int *d=&c;

  b= &a; 
  *b= 1; 
  printf("%d\n",a);

  *d= -1; 
  printf("%d\n",c);

  return 0 ;
}