#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c70201c12db9");

static int __init hello_world_init(void)
{
	pr_debug("Hello World!");
	return 0;
}

static void __exit hello_world_exit(void)
{
	pr_debug("Bye!\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

