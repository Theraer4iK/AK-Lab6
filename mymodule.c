#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/bug.h>

static int param = 0;
module_param(param, int, 0);
MODULE_PARM_DESC(param, "A parameter to test kmalloc and BUG_ON functionality");

static int __init mymodule_init(void) {
    int i;
    int *arr;

    printk(KERN_INFO "MyModule: Initialization started\n");

    if (param < 1 || param > 100) {
        printk(KERN_ERR "MyModule: Invalid parameter, triggering BUG_ON()\n");
        BUG_ON(param < 1 || param > 100);
    }

    arr = kmalloc_array(param, sizeof(int), GFP_KERNEL);
    if (!arr) {
        printk(KERN_ERR "MyModule: Memory allocation failed for array\n");
        return -ENOMEM;
    }

    for (i = 0; i < param; i++) {
        if (i == 5) {
            printk(KERN_ERR "MyModule: Simulating kmalloc failure for element %d\n", i);
            kfree(arr);
            return -ENOMEM;
        }
        arr[i] = i;
    }

    printk(KERN_INFO "MyModule: Ready to process parameters\n");
    kfree(arr);
    return 0;
}

static void __exit mymodule_exit(void) {
    printk(KERN_INFO "MyModule: Cleanup started\n");
    printk(KERN_INFO "MyModule: Cleanup completed\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sorbat");
MODULE_DESCRIPTION("A module to simulate kmalloc failures and use BUG_ON()");
