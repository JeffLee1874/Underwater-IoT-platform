#include "vnode.h"
#include "phy_switcher.h"

#include <linux/netdevice.h>
#include <net/net_namespace.h>

int vnode_register(struct vport *vport, struct vnode *vnode, char* dev_name){
    int err;

    if(!lookup_by_name(dev_name)){
        err = -1;
        return err;
    }

    printk("Found in dev_tree");

    // init_net is the default netname space in Linux, we can create another new netname space.
    vport->dev = dev_get_by_name(&init_net, dev_name);
    if(!vport->dev){
        err = -1;
        return err;
    }
    
    err = 0;
    printk(vport->dev->name);
    return err;
    // err = netdev_rx_handler_register(vport->dev, netdev_frame_hook, vport);
}