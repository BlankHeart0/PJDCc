void swap(int*a,int* b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}

int arr[2];

int main()
{
    int x=-1;
    int y=2;

    printf("x=%d,y=%d\n",x,y);
    swap(&x,&y);
    printf("x=%d,y=%d\n",x,y);

    arr[0]=10;
    arr[1]=100;

    printf("arr[0]=%d,arr[1]=%d\n",arr[0],arr[1]);
    printf("Address:arr[0]=%ld,arr[1]=%ld\n",&arr[0],&arr[1]);

    swap(&arr[0],&arr[1]);
    printf("arr[0]=%d,arr[1]=%d\n",arr[0],arr[1]);

    return 0;
}