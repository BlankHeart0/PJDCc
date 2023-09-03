#include<stdio.h>
#include <string.h>
#include <stdlib.h>

char expression[500];
int tail = 0;
double calculate_value = 111;
int calculate_flag = 0;
int div_zero_flag = 0;

int IsNum(char c)
{
    if (c >= '0' && c <= '9')return 1;
    return 0;
}

int IsOp(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')return 1;
    return 0;
}

int IsPlusSub(char c)
{
    if (c == '+' || c == '-')return 1;
    return 0;
}

int IsMulDiv(char c)
{
    if (c == '*' || c == '/')return 1;
    return 0;
}

double ParseValue(int begin, int end, int* flag)
{
    if (IsPlusSub(expression[begin]) == 0 && IsNum(expression[begin]) == 0)return 0;

    int PositiveOrNegative_flag = 1;
    if (expression[begin] == '-')PositiveOrNegative_flag = -1;
    if (IsPlusSub(expression[begin]))begin++;

    if (begin == end)return 0;

    int dot_i = -1;
    for (int i = begin; i < end; i++)
    {
        if (expression[i] == '.')
        {
            if (dot_i != -1)return 0;
            dot_i = i;
        }
    }
    
    if (dot_i == begin || dot_i == end - 1)return 0;

    if (dot_i == -1)dot_i = end;

    double left = 0;
    for (int i = begin; i < dot_i; i++)
    {
        if (!IsNum(expression[i]))return 0;
        left = left * 10 + expression[i] - '0';
    }

    double right = 0;
    for (int i = end- 1; i> dot_i; i--)
    {
        if (!IsNum(expression[i]))return 0;
        right=right/10+ expression[i] - '0';
    }

    *flag = 1;
    return PositiveOrNegative_flag*(left + right / 10);
}

double Calculate_Part(double* value,char* op,int begin,int end)
{
    double part_value = value[begin];
    for (int i = begin; i < end; i++)
    {
        switch (op[i])
        {
            case '+':part_value += value[i + 1]; break;
            case '-':part_value -= value[i + 1]; break;
            case '*':part_value *= value[i + 1]; break;
            case '/':if (value[i + 1] != 0)part_value /= value[i + 1];
                    else div_zero_flag = 1;break;
        }
    }
    return part_value;
}

double Calculate()
{
    if (tail > 0)
    {
        if (!IsNum(expression[tail - 1]))return calculate_value;

        double value[200]; int value_n = 0;
        char op[200]; int op_n = 0;

        int expression_i = 0;

        if (IsOp(expression[0]))
        {
            value[value_n++] = calculate_value;
            op[op_n++] = expression[expression_i++];
        }

        for (; expression_i < tail;)
        {
            int value_end = expression_i;
            if (IsPlusSub(expression[value_end]))value_end++;
            while (value_end<tail&&!IsOp(expression[value_end]))value_end++;

            int parse_flag = 0;
            double item = ParseValue(expression_i, value_end, &parse_flag);
            printf("item = %.3f  parse_flag = %d\n", item,parse_flag);
            if (parse_flag)
            {
                value[value_n++] = item;
            }
            else return calculate_value;

            if (value_end < tail-1)
            {
                op[op_n++] = expression[value_end];
            }
            expression_i = value_end + 1;
        }

        //test 
        //19.5*6+7*-88.123
        //-3232*7*77/9-98++93*+0.998/-1+000.123
        //1+5*6*7+7-9/3/2
        printf("\n value = \n");
        for (int i = 0; i < value_n; i++)
        {
            printf("%.3f ", value[i]);
        }
        printf("\n op = \n");
        for (int i = 0; i < op_n; i++)
        {
            printf("%c ", op[i]);
        }
        printf("\n");

        //first scan
        double temp_value[200]; int temp_value_n = 0;
        char temp_op[200]; int temp_op_n = 0;
        for (int i = 0; i < op_n; )
        {
            if (IsPlusSub(op[i]))
            {
                temp_value[temp_value_n++] = value[i];
                temp_op[temp_op_n++] = op[i];
                i++;
            }
            else if (IsMulDiv(op[i]))
            {
                int end = i;
                while (end < op_n && IsMulDiv(op[end]) == 1)end++;
                double part_value = Calculate_Part(value, op, i, end);
                if(div_zero_flag)return calculate_value;
                temp_value[temp_value_n++] = part_value;
                if(end<op_n)temp_op[temp_op_n++] = op[end];
                i = end + 1;
            }
        }

        if (op_n>0&&IsPlusSub(op[op_n - 1]))temp_value[temp_value_n++] = value[value_n - 1];

        printf("\n temp_value = \n");
        for (int i = 0; i < temp_value_n; i++)
        {
            printf("%.3f ", temp_value[i]);
        }
        printf("\n temp_op = \n");
        for (int i = 0; i < temp_op_n; i++)
        {
            printf("%c ", temp_op[i]);
        }
        printf("\n");

        //second scan
        calculate_flag = 1;
        if (value_n == 1 && op_n == 0)return value[0];
        return Calculate_Part(temp_value, temp_op, 0, temp_op_n);
    }
    return calculate_value;
}


int main()
{
    scanf_s("%s", expression,500);
    tail = strlen(expression);
    printf("len = %d\n", tail);

    calculate_value = Calculate();

    if (calculate_flag)
    {
        printf("Result = %.6f\n", calculate_value);
        calculate_flag = 0;
    }
    else if (div_zero_flag)
    {
        printf("History  = %.6f\n", calculate_value);
        printf("Zero cannot be divisor ! Please retype ... ...\n");
        div_zero_flag = 0;
    }
    else
    {
        tail = 0;
        printf("History  = %.6f\n", calculate_value);
        printf("Your expression is invalid ! Please retype ... ...\n");
    }

    return 0;

}
