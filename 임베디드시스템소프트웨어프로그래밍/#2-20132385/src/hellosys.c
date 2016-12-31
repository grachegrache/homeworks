#include <linux/unistd.h>

#define __NR_hellosyscall1 313
#define __NR_hellosyscall2 314

int hellosyscall1(int n){
	return syscall(__NR_hellosyscall1, n);
}

int hellosyscall2(int n, int m){
	return syscall(__NR_hellosyscall2, n, m);
}
