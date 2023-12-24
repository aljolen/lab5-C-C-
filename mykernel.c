#include <linux/module.h>
#include <linux/timekeeping.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#define BUFSIZE  100

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Barynina Alina IO-13");

static struct proc_dir_entry *ent;

static ssize_t myread(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
char buf[BUFSIZE];
struct timespec64 current_time;
struct tm tm_info;
int len = 0;

if (*ppos > 0 || count < BUFSIZE)
return 0;

getnstimeofday64(&current_time);

time64_to_tm(current_time.tv_sec, 0, &tm_info);

len += sprintf(buf, "Current Time: %02d:%02d:%02d\n", tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);

if (copy_to_user(ubuf, buf, len + 1))
return -EFAULT;

*ppos = len;
return len;
}

static struct file_operations myops =
        {
                .owner = THIS_MODULE,
                .read = myread,
        };

static int simple_init(void)
{
    ent = proc_create("mydev", 0660, NULL, &myops);
    printk(KERN_ALERT "Module started\n");
    return 0;
}

static void simple_cleanup(void)
{
    proc_remove(ent);
    printk(KERN_WARNING "Module removed\n");
}

module_init(simple_init);
module_exit(simple_cleanup);
