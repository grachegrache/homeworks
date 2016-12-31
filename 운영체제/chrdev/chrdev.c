#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

#define BUFF_SIZE 1024

static char* dev_name = NULL;
static int dev_num = 0;
module_param(dev_name, charp, 0);
module_param(dev_num, int, 0);

static char* buffer = NULL;
static int data_size = 0;

static int virtual_device_open(struct inode* inode, struct file* fp){
    printk("[VD] virtual_device_open\n");
    
    return 0;
}

static int virtual_device_release(struct inode* inode, struct file* fp){
    printk("[VD] virtual_device_release\n");
    
    return 0;
}

static ssize_t virtual_device_write(struct file* fp, const char* buf, size_t count, loff_t* f_ops){
    printk("[VD] virtual_device_write\n");
    
    //[현재 버퍼에 찬 데이터 크기 + 요청 된 데이터 크기]가 버퍼 사이즈보다 크면 count 제한
    if(count+data_size > BUFF_SIZE)
        count = BUFF_SIZE - data_size;
        
    copy_from_user(buffer+data_size, buf, count);
    data_size += count;
    
    return count;
}

static ssize_t virtual_device_read(struct file* fp, char* buf, size_t count, loff_t* f_ops){
    printk("[VD] virtual_device_read\n");
    int read_size = data_size;
    
    copy_to_user(buf, buffer, data_size);
    //버퍼 초기화
    memset(buffer, 0, BUFF_SIZE);
    data_size = 0;
    
    return read_size;
}

static struct file_operations device_fops = {
    .read = virtual_device_read,
    .write = virtual_device_write,
    .open = virtual_device_open,
    .release = virtual_device_release
};

int __init device_init(void){
    printk("[VD] device_init\n");
    
    //장치 명 할당과 장치 명, 주 번호 커널 출력
    if(dev_name == NULL)
        dev_name = "chrdev";
    printk("[VD] dev_name: %s\n", dev_name);
    printk("[VD] dev_num: %d\n", dev_num);
    
    //장치 등록 및 버퍼 공간 할당
    register_chrdev(dev_num, dev_name, &device_fops);
    buffer = (char*) kmalloc(BUFF_SIZE, GFP_KERNEL);
    memset(buffer, 0, BUFF_SIZE);
    
    return 0;
}

void __exit device_exit(void){
    printk("[VD] device_exit\n");
    
    //장치 해제 및 버퍼 공간 해제
    unregister_chrdev(dev_num, dev_name);
    kfree(buffer);
    
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
