#include <linux/init.h>
#include <linux/module.h>

static int __init hello_world_init(void)
{
	pr_debug("Hello World!");
	return 0;
}

static void __exit hello_world_initufs_exit(void)
{
	pr_debug("Bye!");
}

module_init(hello_world_init);
module_exit(hello_world_initufs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("c70201c12db9");
