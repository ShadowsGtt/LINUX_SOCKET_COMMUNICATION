/*************************************************************************
	> File Name: 1.c
	> Author: 
	> Mail: 
	> Created Time: 2017年04月23日 星期日 09时33分01秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
void f(char a[]);
int f1(char x);

int f1(char x)
{
    if(x == ',' || x == ';')
        return 0;
    else if(x == '.' || x == ' ')
        return 0;
    else if(x == '"' || x == '!')
        return 0;
    else 
        return 1;
    
}
void f(char a[])
{
    char b[10][10] = {'\0'};
    int bi = 0;
    int bj = 0;

    for(int i = 0; i < strlen(a); i++)
    {
       if(  f1(a[i]) )
        {
            b[bi][bj++] = a[i];
        }
        else
        {
                bi++;
                bj = 0;
                continue;
        }
    }
    printf("start:\n");
    for(int i = 0; i <= bi; i++)
    {
        printf("%s\n",b[i]);
    }
}

int main()
{
    char a[20] = {'\0'};
    printf("input the char:");
    scanf("%s",a);
    f(a);
    return 0;
}
