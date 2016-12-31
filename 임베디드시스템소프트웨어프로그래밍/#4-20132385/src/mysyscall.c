#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_mysyscall(int n, int m){
	printk("hello raspberry syscall\n");
	return n*m;
}
