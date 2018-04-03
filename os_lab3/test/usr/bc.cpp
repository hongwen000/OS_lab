//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"

#define SIZE 50
class calc{
public:
    calc(char* infix)
    {
        infix_to_postfix(infix);
    }
    int eval_postfix()
    {
        char ch;
        int i = 0, op1, op2;
        while ((ch = postfix[i++]) != 0) {
            if (isdigit(ch))
                push(ch - '0');
            else {
                op2 = pop();
                op1 = pop();
                switch (ch) {
                    case '+':
                        push(op1 + op2);
                        break;
                    case '-':
                        push(op1 - op2);
                        break;
                    case '*':
                        push(op1 * op2);
                        break;
                    case '/':
                        push(op1 / op2);
                        break;
                }
            }
        }
        return s[top];
    }

private:
    char s[SIZE];
    int top = -1;
    char postfix[SIZE];
    void RemoveSpaces(char* source)
    {
        char* i = source;
        char* j = source;
        while (*j != 0) {
            *i = *j++;
            if (*i != ' ')
                i++;
        }
        *i = 0;
    }
    void push(char elem)
    {
        s[++top] = elem;
    }
    char pop()
    {
        return (s[top--]);
    }
    int pr(char elem)
    {
        switch (elem) {
            case '#':
                return 0;
            case '(':
                return 1;
            case '+':
            case '-':
                return 2;
            case '*':
            case '/':
                return 3;
        }
        return -1;
    }
    void infix_to_postfix(char* infix)
    {
        char ch, elem;
        int i = 0, k = 0;
        RemoveSpaces(infix);
        push('#');
        while ((ch = infix[i++]) != '\0') {
            if (ch == '(')
                push(ch);
            else if (isalnum(ch))
                postfix[k++] = ch;
            else if (ch == ')') {
                while (s[top] != '(')
                    postfix[k++] = pop();
                elem = pop();
            } else {
                while (pr(s[top]) >= pr(ch))
                    postfix[k++] = pop();
                push(ch);
            }
        }
        while (s[top] != '#'){
            postfix[k++] = pop();
        }
        postfix[k] = 0;
    }

};

extern "C" void main()
{
    char infix[SIZE];
    printf("Basic calculate 1.0\n");
    gets(infix);
    printf("%s\n", infix);
    calc c(infix);
    printf("%d\n",c.eval_postfix());
}
