//
// Created by 李新锐 on 31/03/2018.
//

#include "../libc/stdio.h"

#include <stdio.h>

void main()
{
    char op;
    int num1, num2, result = 0;
    printf("Basic calculate ver 0.001\n");
    scanf("%d %c %d", &num1, &num2);
    scanf("%s", &op);
    switch(op)
    {
            case '+': result = num1 + num2;
            break;
            case '-': result = num1 - num2;
            break;
            case '*': result = num1 * num2;
            break;
            case '/': result = num1 / num2;
            break;
            default : printf("wrong input");
            break;
    }
        printf("%d\n", result);
}