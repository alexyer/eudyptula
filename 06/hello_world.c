#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

/*
 * Version information
 */
#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Olexander Yermakov <mannavard1611@gmail.com>"
#define DRIVER_DESC "Simple char device"
#define DRIVER_LICENSE "GPL"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "eudyptula"
#define BUF_LEN 80
#define EUDYPTULA_ID "c70201c12db9"

static char msg[BUF_LEN];
static char *id = EUDYPTULA_ID;

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
};

static struct miscdevice my_dev;

int init_module(void)
{
	int retval;

	my_dev.minor = MISC_DYNAMIC_MINOR;
	my_dev.name = DEVICE_NAME;
	my_dev.fops = &fops;
	retval = misc_register(&my_dev);

	if (retval) {
		pr_debug("error: %i", retval);
		return retval;
	}

	pr_debug("minor: %i\n", my_dev.minor);

	return 0;
}

void cleanup_module(void)
{
	misc_deregister(&my_dev);
}

static ssize_t device_read(struct file *filp,
	char *buffer,
	size_t length,
	loff_t *offset)
{
	return simple_read_from_buffer(buffer, length, offset, id,
			strlen(id));
}

static ssize_t
device_write(struct file *filp,
	const char *buffer,
	size_t length,
	loff_t *offset)
{
	if (copy_from_user(msg, buffer, length))
		return -EFAULT;

	if (!strncmp(msg, EUDYPTULA_ID, strlen(EUDYPTULA_ID)) &&
			length - 1 == strlen(EUDYPTULA_ID))
		return length;

	return -EINVAL;
}

