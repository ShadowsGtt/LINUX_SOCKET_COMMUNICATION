/*************************************************************************
	> File Name: 矩阵连乘.c
	> Author: 
	> Mail:  
	> Created Time: 2017年04月13日 星期四 06时01分31秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>

void Tra(int i, int j, int s[][7]);
void Rec(int p[],int num);


void Rec(int p[],int num)
{
    int m[7][7]={0};
    int s[7][7]={0};
    int i,j,l,k;

    for(i = 0;i < num;i++)
        m[i][i] = 0;
    for(l = 2; l < num; l++)
    {
        for(i = 1; i < num-l+1 ; i++)
        {
            j = l-1+i;
            int se =1;

            for(k = i; k < j ;k++)
            {
                if(se == 1)
                {
                    m[i][j] = m[i][k] +m[k+1][j] +p[i-1]*p[k]*p[j];
                    se = 2;
                    s[i][j] = k;
                }
                else
                {
                    int tmp = m[i][k] +m[k+1][j] +p[i-1]*p[k]*p[j];
                    if(tmp < m[i][j])
                    {
                        m[i][j] = tmp;
                        s[i][j] = k;
                    }
                }
            }
        }
        
    }
    printf("\n");
    for(i = 1;i < num ;i++)
    {
        for(j = 1; j < num ;j++)
        {
            printf("%-7d",m[i][j]);
        }
        printf("\n");
    }

    printf("\n");
    for(i = 1;i < num ;i++)
    {
        for(j = 1; j < num ;j++)
        {
            printf("%-7d",s[i][j]);
        }
        printf("\n");
    }
    Tra(1,6,s);
    
}



void Tra(int i, int j, int s[][7])
{
    if(i == j)
    {
        printf("A%d",i);
        return;
    }
    printf("(");
    Tra(i,s[i][j],s);
    Tra(s[i][j]+1,j,s);
    printf(")");
}
int main()
{
    int p[]={30,35,15,5,10,20,25};
    int num = sizeof(p)/sizeof(int);
    Rec(p,num);
    return 0;
}
