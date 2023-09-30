int arr1[10]={0,10,2,'A'};
char str[10]={'h','e','l','l','o','\0'};
long ll[20]={89318,323,923};

int t=9;
long k[1];
char c='u';

int main()
{
    int i;
    for(i=0;i<10;i++)
    {
        printf("arr1[%d]=%d\n",i,arr1[i]);
    }

    printf("%s",str);

    for(i=0;i<24;i++)
    {
        printf("ll[%d]=%ld\n",i,ll[i]);
    }

    printf("t=%d\n",t);

    printf("c=%c:%d\n",c,c);

    printf("k[0]=%ld\n",k[0]);

    return 0;
}