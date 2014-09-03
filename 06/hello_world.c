#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/fs.h>

/*
 * Version information
 */
#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Olexander Yermakov <mannavard1611@gmail.com>"
#define DRIVER_DESC "Hello World module"
#define DRIVER_LICENSE "GPL"

#define DEVICE_NAME "eudyptula"
#define EUDYPTULA_ID "c70201c12db9"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

static int major;
static int device_open = 0;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
}

static void __exit hello_world_exit(void)
{
	pr_debug("Bye!\n");
}

static int __init hello_world_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &fops);

	if (major < 0) {
		pr_debug("Register character device failed with %d\n", major);
		return major;
	}

	pr_debug("Hello, Linux!");
	return 0;
}

void cleanup_module(void)
{
	int ret = unregister_chrdev(major, DEVICE_NAME);
	if (ret < 0)
		pr_debug("Error in unregister_chrdev: %d\n", ret);
}

static int device_open(struct inode *inode, struct file *file)
{
	if(device_open)
		return -EBUSY;
	device_open++;
	sprintf(msg, EUDYPTULA_ID);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	device_open--;
	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp, char *buffer,
		size_t length, loff_t *offset)
{
  int bytes_read = 0;

  if (*msg_Ptr == 0)
    return 0;

  while (length && *msg_Ptr) {

    put_user(*(msg_Ptr++), buffer++);

    length--;
    bytes_read++;
        }

  return bytes_read;
}

static struct usb_device_id usb_kbd_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, usb_kbd_id_table);

module_init(hello_world_init);
module_exit(hello_world_exit);

