kiss random;
kiss srand;
kiss time;

int main()
{
    srand(time(0));

    int key=random()%10;
    int answer;

    while(answer!=key)
    {
        printf("Please input your answer(0-9):\n");

        scanf("%d",&answer);

        if(answer>key)
        {
            printf("Your anwser is greater than the key. Try again...\n");
        }
        else if(answer<key)
        {
            printf("Your anwser is less than the key. Try again...\n");
        }
        else if(answer==key)
        {
            printf("Bingo!!! You are true, good luck!\n");
        }
    }    

    return 0;
}