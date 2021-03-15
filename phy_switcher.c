#include "phy_switcher.h"

#include <linux/module.h>
#include <linux/slab.h>


int lookup_by_name(char* name)
{
    return 1;
}

int forward_device_create(char* forward_dev_name)
{
    struct phy_switcher ps;

    // initialize the forward device
    ps.switcher_name = forward_dev_name;
    ps.entry = (struct list_head*)kmalloc(sizeof(struct list_head), GFP_KERNEL);
    // struct work *w = container_of(work, typeof(*w), work_test);
    return 1;
}