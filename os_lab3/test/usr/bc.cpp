//
// Created by 李新锐 on 31/03/2018.
//

#include "../include/defines.h"
#include "../libc/stdio.h"
#include "../libc/ctype.h"

#define SIZE 50
template <typename T>
struct fixed_stack {
    T stack[SIZE];
    int ele_cnt = 0;
    void push(T ch)
    {
        stack[ele_cnt] = ch;
        ++ele_cnt;
    }
    T top()
    {
        return stack[ele_cnt - 1];
    }
    void pop()
    {
        ele_cnt--;
    }
};

class calc{
public:
    calc (const char * input)
    {
        errorn = parse(input);
        infix2pofix(parsed_input);
    }
    int get_err()
    {
        return errorn;
    }

    inline int get_result()
    {
        return eval_pofix();
    }

private:
    char parsed_input[SIZE];
    char pofix[SIZE];
    fixed_stack<int> s;
    int errorn;
    inline int priority(char ch)
    {
        if (ch == '*' || ch == '/')
            return 4;
        else if (ch == '+' || ch == '-')
            return 3;
        else if (ch == '#')
            return 2;
        else
            return 1;
    }
    int parse(const char* infix)
    {
        int k = 0;
        char ch;
        for(size_t i = 0; i < strlen(infix); ++i)
        {
            ch = infix[i];
            if(isspace(ch))
            {
                continue;
            }
            else if (isdigit(ch) || ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '/')
            {
                parsed_input[k++] = ch;
            }
            else
            {
                return i + 1;
            }
        }
        return 0;
    }
    void infix2pofix(const char* infix_str)
    {
        char ch;
        s.push('#');
        int j = 0;
        for(size_t i = 0; i < strlen(infix_str); ++i)
        {
            ch = infix_str[i];
            if (ch == '(')
            {
                s.push('(');
            }
            else if (ch == ')')
            {
                while (s.top() != '(')
                {
                    pofix[j++] = s.top();
                    s.pop();
                }
                s.pop();    //移除'('
            }
            else if (isdigit(ch))
            {
                pofix[j++] = ch;
            }
            else
            {
                while (priority(ch) < priority(s.top()))
                {
                    pofix[j++] = s.top();
                    s.pop();
                }
                s.push(ch);
            }
        }
        while (s.top() != '#')
        {
            pofix[j++] = s.top();
            s.pop();
        }
        pofix[j] = '\0';
    }
    int eval_pofix()
    {
        int op1 = 0;
        int op2 = 0;
        char ch;
        for(size_t i = 0; i < strlen(pofix); ++i)
        {
            ch = pofix[i];
            if (isdigit(ch))
            {
                s.push(ch - '0');
            } else{
                op1 = s.top();
                s.pop();
                op2 = s.top();
                s.pop();
                switch (ch)
                {
                    case '+':
                    {
                        s.push(op1 + op2);
                        break;
                    }
                    case '-':
                    {
                        s.push(op2 - op1);
                        break;
                    }
                    case '*':
                    {
                        s.push(op1 * op2);
                        break;
                    }
                    case '/':
                    {
                        s.push(op2 / op1);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
        return s.top();
    }
};

extern "C" void main()
{
//    char infix[SIZE];
//    printf("Basic calculate 1.1\n");
//    printf("Input q to exit\n");
//    printf("I\n");
//    putchar('I');
    while (true)
    {
//        gets(infix);
//        if (strcmp(infix, "q") == 0)
//            break;
//        calc c(infix);
//        int errorn = c.get_err();
//        if(errorn != 0)
//        {
//            printf("Wrong input at column %d\n", errorn);
//        }
//        else
//        {
//            printf("%d\n", c.get_result());
//        }
    }
}

