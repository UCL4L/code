#include <stdio.h>

int age(int n)
{
	if (n == 1)
		return 10;

	if (n >= 1)
		return age(n - 1) + 2;

}

void main()
{
	int n;
	printf("请输入共几人:");
	scanf("%d",&n);
	printf("%d",age(n));
}


