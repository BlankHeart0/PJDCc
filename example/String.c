char* s;
char array[8];
int main() {

//1
  printf("Hello world\n");


//2 
  s="Hello world";
  printf("%s\nlen=%d",s,strlen(s));
  //int len=strlen(s);
 // printf("%d\n",len);

//3  BUG
  array[0]='h';
  array[1]='e';
  array[2]='l';
  array[3]='l';
  array[4]='o';
  array[6]='\n';

  printf("%s",array);

  return 0;
}