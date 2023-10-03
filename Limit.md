# Limit

## * Pointer and Array only have one dimension

## * Don't support local array

## * When you initialize global variable or array ,don't use negative

## * Increment and Decrement only used for basic type variable, such as i++ i-- ++j --j

## * Function parameter's max quantity is 6

## * Local scope only have one depth

### Right

```c
int main()
{
    int a=5;
    long b=a+a;
    ... ...
    int i;
    for(i=0;i<b;i++)
    {
        ... ...
    }
}
```

### False

```c
int main()
{
    {
        int a=0; //xxx
    }

    int b=10;
    if(b>0)
    {
        int c=0; //xxx
        ... ...
    }

    for(int i=0;i<10;i++) //xxx
    {
        ... ...
    }

    while(1)
    {
        char* s="local"; //xxx
        ... ...
    }
}
```
