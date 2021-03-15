#ifndef _VNODE_H
#define _VNODE_H

#include <linux/netdevice.h>
#include "vport.h"

struct vnode
{
    /* data */
    float x,y,z;
    float signal_HZ;
};

int vnode_register(struct vport *vport, struct vnode *vnode, char* forward_dev_name);

#endif