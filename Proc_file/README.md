# Project 1: Kernel Module
In this project, you will learn how to create a kernel module and load it into the Linux kernel. 
You will then modify the kernel module so that it creates an entry in the /proc file system

## Task 1 : 
	Design a kernel module that creates a /proc file named /proc/jiffies that reports the current value of jiffies when the /proc/jiffies file is read, such as with the command
	
	cat /proc/jiffies
	Be sure to remove /proc/jiffies when the module is removed.
	
## Task 2 : 	
	Design a kernel module that creates a proc file named /proc/seconds that reports the number of elapsed seconds since the kernel module was loaded. This will involve using the value of jiffies as well as the HZ rate. When a user enters the command
	cat /proc/seconds
	your kernel module will report the number of seconds that have elapsed since the kernel module was first loaded. Be sure to remove /proc/seconds when the module is removed.
