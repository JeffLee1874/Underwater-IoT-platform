#ifndef _VNODE_H
#define _VNODE_H

#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/dst_metadata.h>
#include <net/net_namespace.h>
#include <linux/list.h>
#include "typedef.h"
#include "vport.h"
#include "phy_switcher.h"


int vnode_register(struct vnode *vnode, char* dev_name, struct phy_switcher* ps, struct vport* vport);

#endif