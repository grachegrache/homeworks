#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_hellosyscall1(int n){
	printk("Hello Syscall\n");
	return n*n;
}
