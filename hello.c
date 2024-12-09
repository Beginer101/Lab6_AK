// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Deykalo Vladyslav <vladdejkalo@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0644);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!' (default=1)");

LIST_HEAD(time_list);

struct time_data {
	ktime_t start_time;
	struct list_head list;
};

__attribute__((__noinline__))
static void add_to_list(struct list_head *node)
{
	BUG_ON(!node);
	list_add_tail(node, &time_list);
}

static int __init hello_init(void)
{
	uint i;

	BUG_ON(print_count > 10);

	if (print_count == 0 || (print_count >= 5 && print_count <= 10))
		pr_warn("Warning: print_count is %u\n", print_count);

	for (i = 0; i < print_count; i++) {
		struct time_data *time_data;

		if (i == 4)
			time_data = NULL;
		else
			time_data = kmalloc(sizeof(*time_data), GFP_KERNEL);
			if (!time_data)
				break;

		pr_emerg("Hello, world! %u\n", i+1);

		if (time_data)
			add_to_list(&time_data->list);
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct time_data *pos, *tmp;

	list_for_each_entry_safe(pos, tmp, &time_list, list) {
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello_init);
module_exit(hello_exit);
