#include "vnode.h"


// #include <linux/netdevice.h>

// #include <linux/hashtable.h>

// /* Returns null if this device is not attached to a datapath. */
// struct vport *netdev_get_vport(struct net_device *dev)
// {
// 	if (likely(dev->priv_flags & IFF_OVS_DATAPATH))
// 		return (struct vport *)
// 			rcu_dereference_rtnl(dev->rx_handler_data);
// 	else
// 		return NULL;
// }



static rx_handler_result_t netdev_frame_hook(struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;

	if (unlikely(skb->pkt_type == PACKET_LOOPBACK))
		return RX_HANDLER_PASS;

#ifndef USE_UPSTREAM_TUNNEL
	vport_received(skb, NULL);
#else
	vport_received(skb, skb_tunnel_info(skb));
#endif
	return RX_HANDLER_CONSUMED;
}

int vnode_register(struct vnode *vnode, char* dev_name, struct phy_switcher* ps, struct vport* vport){
    int err;
    vport = (struct vport*)kmalloc(sizeof(struct vport), GFP_KERNEL);
    if(!vport)
    {
        err = 0;
        printk("vport generate failure!");
        return err;
    }

    if(search_vport_by_name(ps, dev_name, vport)){
        err = 0;
        printk("%s has enlisted!", dev_name);
        return err;
    }

    // init_net is the default netname space in Linux, we can create another new netname space.
    vnode->dev = dev_get_by_name(&init_net, dev_name);
    if(!vnode->dev){
        err = 0;
        printk("%s is not in the netname_space", dev_name);
        return err;
    }
    
    err = netdev_rx_handler_register(vnode->dev, netdev_frame_hook, vport);
    if(err)
    {
        printk("netdev_rx_handler_register failed! Failed code: %d /n", err);
        return err;
    }

    vport->name = dev_name;
    vport->vnode = vnode;
    vport->phy_switcher = ps;
    // hash_add(ps->entry, vport->hash_node, hash_name(dev_name));
    // vport->hash_node = (struct hlist_node* )kmalloc(sizeof(struct hlist_node), GFP_KERNEL);
    INIT_HLIST_NODE(&vport->hash_node);
    hlist_add_head(&vport->hash_node, ps->entry);

    err = 1;
    printk("%s register successfully!", vport->name);
    return err;
    // err = netdev_rx_handler_register(vport->dev, netdev_frame_hook, vport);
}