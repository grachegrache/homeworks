#include <linux/unistd.h>
#include <stdio.h>

#define __NR_hellosyscall1 313
#define __NR_hellosyscall2 314

int main(void){
	int n, m;
	n = syscall(__NR_hellosyscall1, 10);
	m = syscall(__NR_hellosyscall2, n, 2);
	printf("hellosyscall1 return value: %d\n", n);
	printf("hellosyscall2 return value: %d\n", m);
	return 0;
}
