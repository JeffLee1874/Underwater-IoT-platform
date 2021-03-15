#ifndef _VPORT_H
#define _VPORT_H

#include <linux/types.h>
#include <linux/rculist.h>

#include "vnode.h"
#include "phy_switcher.h"


struct vport {
	struct net_device *dev;
	struct phy_switcher *phy_switcher;
    
    char *name;
	int port_no;
	struct hlist_node *hash_node;
	struct rcu_head *rcu;
    struct vnode *vnode;
};

//vport create and register to default phy_swithcer
struct vport* vport_create(struct phy_switcher *phy_switcher);
//vport create and register to chosen phy_swithcer
struct vport* vport_create_ps(struct phy_switcher *phy_switcher);
void vport_free(struct vport *vport);

#endif