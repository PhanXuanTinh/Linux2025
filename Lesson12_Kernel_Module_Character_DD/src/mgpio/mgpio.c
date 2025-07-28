#include <linux/module.h>
#include <linux/gpio.h>

#define DEVICE_AUTHOR "TinhPhan"
#define DEVICE_DESC   "Hello world kernel module"
#define DEVICE_VERS   "1.0"

static int __init mgpio_init(void)
{
    pr_info("TinhPhan: hello world kernel module!\n");
    return 0;
}

static void __exit mgpio_exit(void)
{
    pr_info("TinhPhan: goodbye world kernel module!\n");
}

module_init(mgpio_init);
module_exit(mgpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DEVICE_AUTHOR);
MODULE_DESCRIPTION(DEVICE_DESC);
MODULE_VERSION(DEVICE_VERS);
