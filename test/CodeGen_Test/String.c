char* s;
char array[8];
int main() {

//1
  print_string("Hello world\n");


//2 
  s="Hello world\n";
  print_string(s);


//3  BUG
  array[0]='h';
  array[1]='e';
  array[2]='l';
  array[3]='l';
  array[4]='o';
  array[6]='\n';

  print_char(array[3]);
  print_char(*(array+3));
  print_string(array);

  return 0;
}