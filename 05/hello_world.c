#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

/*
 * Version information
 */
#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Olexander Yermakov <mannavard1611@gmail.com>"
#define DRIVER_DESC "Hello World module"
#define DRIVER_LICENSE "GPL"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

static void hello_world_disconnect(struct usb_interface *iface)
{
	pr_debug("Bye!\n");
}

static int hello_world_probe(struct usb_interface *iface,
		 const struct usb_device_id *id)
{
	pr_debug("Hello, USB!");
	return 0;
}

static struct usb_device_id usb_kbd_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, usb_kbd_id_table);

static struct usb_driver hello_world_driver = {
	.name        = "hello_world",
	.probe       = hello_world_probe,
	.disconnect  = hello_world_disconnect,
	.id_table    = usb_kbd_id_table,
};

module_usb_driver(hello_world_driver);

