int main()
{
    int i;
    int j;
    for(i=0;i<10;i++)
    {
        if(i==0)continue;
        for(j=0;j<10;j++)
        {
            if(j==5)break;
            printf("i=%d,j=%d\n",i,j);
            if(i==3)break;
        }
        if(i>6)break;
    }
}
