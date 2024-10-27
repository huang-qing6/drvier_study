/* from drv/char/mem.c */
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>

// device_id 
dev_t dev;
static unsigned char kernel_buf[1024];

static struct class *hello_class;
static struct cdev hello_cdev;


/**func from fs.h
 * 1.open 2.read 3.write 4.release
 */

static int hello_open(struct inode *node, struct file *file){
    printk("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}


// differ usr read(fd, &buf, sizeof(buf))
static ssize_t hello_read (struct file *file, char __user *buf, size_t size, loff_t *offset){
	int err;
    int len = 1024 > size ? size : 1024;
	printk("in !!!!!!!!!!!!!!!!!!!!\n");
	err = copy_to_user(buf, kernel_buf, len);
	return len;
}

// differ usr write(fd, &buf, sizeof(buf))
static ssize_t hello_write (struct file *file, const char __user *buf, size_t size, loff_t *offset){
	int err;
    int len = 1024 > size ? size : 1024;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(kernel_buf, buf, len);
	return len;
}

static int hello_release (struct inode *node, struct file *file){
    printk("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/* create file_op */
static const struct file_operations hello_drv = {
    .owner      = THIS_MODULE,
	.read		= hello_read,
	.write		= hello_write,
	.open		= hello_open,
    .release    = hello_release,
};

/* register chrdev */


/* entry */
static int hello_init(void){
    int ret;

    // register_chrdev
    ret = alloc_chrdev_region(&dev, 0, 1, "hello");
    if(ret < 0){
        printk("alloc chrdev region failed\n");
        return -1;
    }

    cdev_init(&hello_cdev, &hello_drv);
    ret = cdev_add(&hello_cdev, dev, 1);
    if(ret < 0){
        printk("chrdev add failed\n");
        return -1;
    }

    hello_class = class_create(THIS_MODULE, "hello");
    if(IS_ERR(hello_class)){
        printk("fail to allocate class\n");
        return PTR_ERR(hello_class);
    }

    /* build a device named "hello" in the dev */
    device_create(hello_class, NULL, dev, NULL, "hello");

    return 0;
}


/* exit */
static void hello_exit(void){
    device_destroy(hello_class, dev);

    class_destroy(hello_class);

    //unregister_chrdev(major, "mariohuang");
    cdev_del(&hello_cdev);
    unregister_chrdev_region(dev, 1);
}


/* generate a fts node */
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
