#include <stdio.h>
int main() 
{
    int i,j;
    for(i=1;i<=9;i++)
	{
        for(j=0;j<=9;j++)
		{
            if(j<i)
				printf("%d*%d=%-2d  ",i,j+1,i*(j+1));
        }
        printf("\n");  
    }
    return 0;
}
