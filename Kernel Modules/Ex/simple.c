#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gcd.h>     //for gcd function
#include <linux/hash.h>    //for Golden ratio prime value
#include <linux/jiffies.h> //for jiffies value
#include <asm/param.h>     //for HZ value

/**
 * This function is called when the module is loaded.
 */

int simple_init(void)
{
    printk(KERN_INFO "Loading Kernel Module\n");
    printk(KERN_INFO "Gcd: %lu\n", gcd(3300, 24));
    printk(KERN_INFO "Golden Ratio: %lu\n", GOLDEN_RATIO_PRIME);
    printk(KERN_INFO "Jiffies: %ld", jiffies);
    printk(KERN_INFO "HZ value: %d", HZ);
    return 0;
}

/**
 * This function is called when the module is removed.
 */

void simple_exit(void)
{
    printk(KERN_INFO "Jiffies: %ld", jiffies);
    printk(KERN_INFO "Removing Kernel Module\n");
}

/**
 * Macros for registering a module entry and exit points.
 */

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple MOdule");
MODULE_AUTHOR("SGG");