#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_ultrasonic_log(int d, int num, int status) {
	printk("ultrasonic_log called. Distance is %d\n",d);
	printk("set %dth tab to %s\n",num, status?"ON":"OFF");
	return 0;
}
