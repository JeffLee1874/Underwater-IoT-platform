#ifndef _PHY_SWITCHER_H
#define _PHY_SWITCHER_H

#include <linux/types.h>
#include <linux/rculist.h>
#include <linux/list.h>

#include "simulation/simulation.h"
#include "typedef.h"
#include "vport.h"



/**
 * @name: name of vport that you want to look for
 * 
 */
int search_vport_by_name(struct phy_switcher* ps, char* name, struct vport* vport);
int search_and_send(struct phy_switcher* ps, char* name, struct vport* vport, struct sk_buff* skb);
int initialize_ps(struct phy_switcher* ps, char* name, int size);

u32 hash_name(const char *s);

int forward_device_create(char* forward_dev_name);

int unregister_all(struct phy_switcher* ps);

#endif