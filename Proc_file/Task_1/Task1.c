#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/param.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 		128
#define PROC_NAME 		"jiffies"

ssize_t proc_read (struct file *, char __user *, size_t, loff_t *);

static struct proc_ops my_proc_ops = {
	.proc_read = proc_read,
};

int proc_init(void){
	proc_create(PROC_NAME, 0666, NULL, &my_proc_ops);
	return 0;
}

void proc_exit(void){
	remove_proc_entry(PROC_NAME, NULL);
}
ssize_t proc_read (struct file *filee, char __user *usr_buf, size_t count, loff_t *pos){
	int rv =0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;
	if(completed){
		completed =0;	/* to make sure that proc_read() returns 0 */
		return 0;
	}
	completed = 1;
	/* sprintf returns number of printed letters stored in rv */ 
	rv = sprintf(buffer , "jiffies = %lu \n", jiffies);
	
	/*copies kernel space buffer to user buffer */
	copy_to_user(usr_buf, buffer,rv);
	return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alaa Wahba");
MODULE_DESCRIPTION("proc Module");
