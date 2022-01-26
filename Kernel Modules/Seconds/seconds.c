#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

unsigned long start_jiffies = 0;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

//initialize memebers
static const struct proc_ops proc_file_fops = {
    .proc_read = proc_read,
};

// function called when the kernel is loaded
static int __init proc_init(void) 
{
    proc_create(PROC_NAME, 0, NULL, &proc_file_fops);

    printk(KERN_INFO "/proc/%s successfully created\n", PROC_NAME);
    
    return 0;
}

// function called when the kernel is removed
static void __exit proc_exit(void)
 {

    remove_proc_entry(PROC_NAME, NULL);
    printk(KERN_INFO "/proc/%s removed successfully\n", PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "Seconds passed: %lus\n", (jiffies - start_jiffies) / HZ);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
