// int a;
// int b;
// int c;
// int main() {
//   a= 42; b= 19;
//   print_int(a/b);
//   print_int(a%b);
//   print_int(a & b);
//   print_int(a | b);
//   print_int(a ^ b);
//   print_int(1 << 3);
//   print_int(63 >> 3);
//   return 0;
// }



// int   x;

// int main() {
//   x= -23; print_int(x);
//   print_int(-10 * -10);

//   x= 1; x= ~x; print_int(x);

//   x= 2 > 5; print_int(x);
//   x= !x; print_int(x);
//   x= !x; print_int(x);

//   x= 13; if (x) { print_int(13); }
//   x= 0; if (!x) { print_int(14); }


//   return 0;
// }

// char a; char b; char c;
// int  d; int  e; int  f;
// long g; long h; long i;


// int main() {
//   b= 5; c= 7; a= b + c++; print_int(a);
//   e= 5; f= 7; d= e + f++; print_int(d);
//   h= 5; i= 7; g= h + i++; print_int(g);
//   a= b-- + c; print_int(a);
//   d= e-- + f; print_int(d);
//   g= h-- + i; print_int(g);
//   a= ++b + c; print_int(a);
//   d= ++e + f; print_int(d);
//   g= ++h + i; print_int(g);
//   a= b * --c; print_int(a);
//   d= e * --f; print_int(d);
//   g= h * --i; print_int(g);
//   return 0;
// }

int a;
int main()
{
    a=0;
    print_int(a);
    print_int(a++);

    print_int(a);
    print_int(a--);

    print_int(a); 
    print_int(++a);

    print_int(a);
    print_int(--a);

    print_int(a);

    return 0;
}