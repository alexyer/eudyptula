#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
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

int init_module(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);

	if (major < 0) {
		printk("Registering the character device failed with %d\n",
			major);
		return major;
	}

	printk("<1>I was assigned major number %d.  To talk to\n", major);
	printk("<1>the driver, create a dev file with\n");
	printk("'mknod /dev/chardev c %d 0'.\n", major);
	printk("<1>Try various minor numbers.  Try to cat and echo to\n");
	printk("the device file.\n");
	printk("<1>Remove the device file and module when done.\n");

	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(major, DEVICE_NAME);
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
