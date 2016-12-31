#include <linux/unistd.h>
#include <stdio.h>

int main(void){
	int n, m;
	n = hellosyscall1(10);
	m = hellosyscall2(n, 2);
	printf("hellosyscall1 return value: %d\n", n);
	printf("hellosyscall2 return value: %d\n", m);
	return 0;
}
