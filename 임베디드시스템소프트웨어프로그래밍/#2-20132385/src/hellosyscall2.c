#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_hellosyscall2(int n, int m){
	printk("Hello Syscall\n");
	return n*m;
}
