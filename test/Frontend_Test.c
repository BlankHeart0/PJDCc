// int arr[5];
// int* t;
// int main()
// {
//   arr[0]=1;arr[1]=2;arr[2]=3;arr[3]=4;
  
//   print_int(arr[0]++);
//   print_int(arr[0]--);

//   print_int(++arr[0]);
//   print_int(--arr[0]);

//   t=arr;

//   while(*t)
//   {
//     print_int(*t);
//     t++;
//   }

//   return 0;
// }

int a;
int b;
int c;
int main() {
  a= 42; b= 19;
  printint(a & b);
  printint(a | b);
  printint(a ^ b);
  printint(1 << 3);
  printint(63 >> 3);
  return 0;
}



char *str;
int   x;

int main() {
  x= -23; printint(x);
  printint(-10 * -10);

  x= 1; x= ~x; printint(x);

  x= 2 > 5; printint(x);
  x= !x; printint(x);
  x= !x; printint(x);

  x= 13; if (x) { printint(13); }
  x= 0; if (!x) { printint(14); }

  str="Hello world\n"; 
  while (*str ){
    printchar(*str);
    str++;
  }

  return 0;
}


char a; char b; char c;
int  d; int  e; int  f;
long g; long h; long i;


int main() {
  b= 5; c= 7; a= b + c++; printint(a);
  e= 5; f= 7; d= e + f++; printint(d);
  h= 5; i= 7; g= h + i++; printint(g);
  a= b-- + c; printint(a);
  d= e-- + f; printint(d);
  g= h-- + i; printint(g);
  a= ++b + c; printint(a);
  d= ++e + f; printint(d);
  g= ++h + i; printint(g);
  a= b * --c; printint(a);
  d= e * --f; printint(d);
  g= h * --i; printint(g);
  return 0;
}