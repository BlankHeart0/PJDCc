  // char  a; char *b; char  c;
  // int   d; int  *e; int   f;
// int main() {

//   char  a; char *b; char  c;
//   int   d; int  *e; int   f;
//   a= 18; printf("%d\n",a);
//   b= &a; c= *b; printf("%d\n",c);

//   d= 12; printf("%d\n",d);
//   e= &d; f= *e; printf("%d\n",f);
  
//   return 0;
// }


int   a;
int  *b;
int main()
{

char  c;
char *d;
  b= &a; *b= 15; printf("%d\n",a);
  d= &c; *d= 16; printf("%d\n",c);
  return 0 ;
}