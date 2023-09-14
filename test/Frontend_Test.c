int   a;
int   b;
int  *a_ptr;
int  *b_ptr;

int main() {
  a= 12; b=18; printint(c);printint(d);

  a_ptr= &a;      printint(*a_ptr);
  a_ptr= &a + 1;  printint(*a_ptr);
  a_ptr= &a;      printint(*(a_ptr+1));

  printint(111111111);
  
  b_ptr= &b;      printint(*b_ptr);
  b_ptr= &b - 1;  printint(*b_ptr);
  b_ptr= &b;      printint(*(b_ptr-1));

  return 0;
}