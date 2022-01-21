#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

//initialize memebers
static struct file_operations proc_file_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};


unsigned long start_jiffies = 0;

ssize_t proc_read(struct file *file, char __user *usr_buf, ssize_t count, loff_t *pos)
{
    int unknown = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }

    completed = 1;
    unknown = sprintf(buffer, "Elapsed seconds: %lus\n", (jiffies - start_jiffies) / HZ);
    raw_copy_to_user(usr_buf, buffer, unknown);
    return unknown;
}

static int __init proc_init(void) {
    proc_create(PROC_NAME, 0644, NULL, &proc_file_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit proc_exit(void) {
    proc_remove(our_proc_file);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");