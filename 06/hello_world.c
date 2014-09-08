#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

/*
 * Version information
 */
#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Olexander Yermakov <mannavard1611@gmail.com>"
#define DRIVER_DESC "Simple char device"
#define DRIVER_LICENSE "GPL"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "eudyptula"
#define BUF_LEN 80
#define EUDYPTULA_ID "c70201c12db9"
#define DYNAMIC_MINOR 73

static int major;
static int dev_open = 0;
static char msg[BUF_LEN];
static char *msg_ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static struct miscdevice my_dev;

int init_module(void)
{
	int retval;
	my_dev.minor = DYNAMIC_MINOR;
	my_dev.name = DEVICE_NAME;
	my_dev.fops = &fops;
	retval = misc_register(&my_dev);

	if (retval){
		printk("error: %i", retval);
		return retval;
	}

	printk("minor: %i", my_dev.minor);

	return 0;
}

void cleanup_module(void)
{
	misc_deregister(&my_dev);
}

static int device_open(struct inode *inode, struct file *file)
{
	if (dev_open)
		return -EBUSY;
	dev_open++;
	sprintf(msg, EUDYPTULA_ID);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	dev_open--;
	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp,
         char *buffer,
         size_t length,
         loff_t * offset)
{
	int bytes_read = 0;

	if (*msg_ptr == 0)
		return 0;

	while (length && *msg_ptr) {
		put_user(*(msg_ptr++), buffer++);

		length--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk("%.*s", len, buff);

	if (!strncmp((const char *)buff, EUDYPTULA_ID, strlen(EUDYPTULA_ID)))
		return len;
	else
		return -EINVAL;
}
