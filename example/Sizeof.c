int a1;
char a2;
long a3;

int* b1;
char* b2;
long* b3;

int arr1[10];
long arr2[40];
long arr3[4];

int main()
{
    int c1;
    char c2;
    long c3;

    int* d1;
    char* d2;
    long* d3;
    
    printf("%d,%d,%d\n",sizeof(a1),sizeof(a2),sizeof(a3));
    printf("%d,%d,%d\n",sizeof(b1),sizeof(b2),sizeof(b3));
    printf("%d,%d,%d\n",sizeof(arr1),sizeof(arr2),sizeof(arr3));

    printf("%d,%d,%d\n",sizeof(c1),sizeof(c2),sizeof(c3));
    printf("%d,%d,%d\n",sizeof(d1),sizeof(d2),sizeof(d3));

    printf("%d,%d,%d\n",sizeof(int),sizeof(char),sizeof(long));
    printf("%d,%d,%d\n",sizeof(int*),sizeof(char*),sizeof(long*));

    return 0;

}
