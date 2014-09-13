#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/module.h>

/*
 * Version information
 */
#define DRIVER_VERSION ""
#define DRIVER_AUTHOR "Olexander Yermakov <mannavard1611@gmail.com>"
#define DRIVER_DESC "Debugfs eudyptula module"
#define DRIVER_LICENSE "GPL"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

static const char *dir_name = "eudyptula";
struct dentry *root_dir;

static int __init hello_world_init(void)
{
	pr_debug("Hello World!");
	root_dir = debugfs_create_dir(dir_name, NULL);

	if (!root_dir) {
		pr_debug("failed to create root directory");
		return 0;
	}

	if (root_dir == -ENODEV) {
		pr_debug("Your kernel build without debugfs support");
		return -ENODEV;
	}

	return 0;
}

static void __exit hello_world_exit(void)
{
	pr_debug("Bye!\n");
	debugfs_remove_recursive(root_dir);
}

module_init(hello_world_init);
module_exit(hello_world_exit);

