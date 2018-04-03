//
// Created by 李新锐 on 04/04/2018.
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
    calc (const char * infix)
    {
        infix2pofix(infix);
    }
    int operator() (){ return eval_pofix();};

private:
    char parsed_str[SIZE];
    char pofix[SIZE];
    fixed_stack<int> s;
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
    void infix2pofix(const char* infix)
    {
        char ch;
        s.push('#');
        int j = 0;
        for(size_t i = 0; i < strlen(parsed_str); ++i)
        {
            ch = infix[i];
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
    }
};
extern "C" void main()
{
    char infix[SIZE];
    printf("Basic calculate 1.0\n");
    gets(infix);
    printf("%d\n",calc(infix));
}
