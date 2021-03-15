#ifndef _PHY_SWITCHER_H
#define _PHY_SWITCHER_H

#include <linux/types.h>
#include <linux/rculist.h>

/**
 * @switcher_name: phy_switcher name
 * @entry: hash list entry
 */
struct phy_switcher
{
    char *switcher_name;
    struct list_head *entry;
};

/**
 * @name: phy_switcher name that you want to look for
 * 
 * This function intends to find that if the phy_switcher name is legal
 */
int lookup_by_name(char* name);

int forward_device_create(char* forward_dev_name);

#endif