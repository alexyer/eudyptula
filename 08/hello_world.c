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

#define BUF_LEN 80
#define PG_SIZE 80*23
#define EUDYPTULA_ID "c70201c12db9"

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);

static char msg[BUF_LEN] = {0};
static char text[PG_SIZE] = {0};
static char *id = EUDYPTULA_ID;

static const char *dir_name = "eudyptula";
static const char *id_file_name = "id";
static const char *jiffies_file_name = "jiffies";
static const char *foo_file_name = "foo";

struct dentry *root_dir;
struct dentry *id_file;
struct dentry *jiffies_file;
struct dentry *foo_file;

int id_file_data;
int jiffies_file_data;
int foo_file_data;

struct semaphore mr_sem;

static ssize_t id_file_read(struct file *filp,
	char *buffer,
	size_t length,
	loff_t *offset)
{
	return simple_read_from_buffer(buffer, length, offset, id,
			strlen(id));
}

static ssize_t id_file_write(struct file *filp,
	const char *buffer,
	size_t length,
	loff_t *offset)
{
	simple_write_to_buffer(msg, BUF_LEN, offset,
			buffer, length);

	if (!strncmp(msg, EUDYPTULA_ID, strlen(EUDYPTULA_ID)) &&
			length - 1 == strlen(EUDYPTULA_ID))
		return length;

	return -EINVAL;
}

static ssize_t foo_file_read(struct file *filp,
	char *buffer,
	size_t length,
	loff_t *offset)
{
	if (down_interruptible(&mr_sem))
		return -EINTR;

	int ret = simple_read_from_buffer(buffer, length, offset, text,
			strlen(text));

	up(&mr_sem);

	return ret;
}

static ssize_t foo_file_write(struct file *filp,
	const char *buffer,
	size_t length,
	loff_t *offset)
{
	if (down_interruptible(&mr_sem))
		return -EINTR;

	int ret = simple_write_to_buffer(text, PG_SIZE, offset,
			buffer, length);

	up(&mr_sem);

	return ret;
}

static ssize_t jiffies_file_read(struct file *filp,
	char *buffer,
	size_t length,
	loff_t *offset)
{
	char jiffies_str[BUF_LEN] = {0};
	snprintf(jiffies_str, sizeof(jiffies_str), "%lu", jiffies);

	return simple_read_from_buffer(buffer, length, offset, jiffies_str,
			sizeof(jiffies_str));
}

static const struct file_operations id_fops = {
	.read = id_file_read,
	.write = id_file_write,
};

static const struct file_operations jiffies_fops = {
	.read = jiffies_file_read,
};

static const struct file_operations foo_fops = {
	.read = foo_file_read,
	.write = foo_file_write,
};

static int __init hello_world_init(void)
{
	sema_init(&mr_sem, 1);
	root_dir = debugfs_create_dir(dir_name, NULL);

	if (!root_dir) {
		pr_debug("failed to create root directory");
		return 0;
	}

	if ((int)root_dir == -ENODEV) {
		pr_debug("Your kernel build without debugfs support");
		return -ENODEV;
	}

	id_file = debugfs_create_file(id_file_name, 0666, root_dir,
			&id_file_data, &id_fops);

	jiffies_file = debugfs_create_file(jiffies_file_name, 0644, root_dir,
			&jiffies_file_data, &jiffies_fops);

	foo_file = debugfs_create_file(foo_file_name, 0644, root_dir,
			&foo_file_data, &foo_fops);

	return 0;
}

static void __exit hello_world_exit(void)
{
	debugfs_remove_recursive(root_dir);
}

module_init(hello_world_init);
module_exit(hello_world_exit);

