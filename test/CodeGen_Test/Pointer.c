//   char  a; char *b; char  c;
//   int   d; int  *e; int   f;
// int main() {


//   a= 18; printint(a);
//   b= &a; c= *b; printint(c);

//   d= 12; printint(d);
//   e= &d; f= *e; printint(f);
  
//   return 0;
// }



long   a;
long   b;
long  *a_ptr;
long  *b_ptr;

int main() {
  a= -1; b=18; print_int(a);print_int(b);
  print_int(111111111);

  a_ptr= &a;      print_int(*a_ptr);
  a_ptr= &a + 1;  print_int(*a_ptr);
  a_ptr= &a;      print_int(*(a_ptr+1));

  print_int(111111111);
  
  b_ptr= &b;      print_int(*b_ptr);
  b_ptr= &b - 1;  print_int(*b_ptr);
  b_ptr= &b;      print_int(*(b_ptr-1));

  return 0;
}

// int   a;
// int  *b;
// char  c;
// char *d;

// int main()
// {
//   b= &a; *b= 15; printint(a);
//   d= &c; *d= 16; printint(c);
//   return 0 ;
// }