#include <stdio.h>

unsigned long factorial(int n) 
{
	if (n <= 0) return 1;
	return n * factorial(n-1);
}

int main()
{
	int n;
	printf("Enter a number: ");
	scanf("%d", &n);
	
	printf("%d! = %lu\n", n, factorial(n));
	return 0;
}

