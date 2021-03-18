#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/dst_metadata.h>
#include <net/net_namespace.h>
#include <linux/list.h>
#include <linux/types.h>

/**
 * @switcher_name: phy_switcher name
 * @entry: hash list entry
 */
struct phy_switcher
{
    char *switcher_name;
    struct hlist_head *entry;
    struct hlist_node *h_node;
};

struct vnode
{
    /* data */
    struct net_device *dev;
    float x,y,z;
    float signal_HZ;
};

struct vport {
	struct phy_switcher *phy_switcher;
    
    char *name;
	int port_no;
	struct hlist_node hash_node;
	struct rcu_head *rcu;
    struct vnode *vnode;
};

struct simulation_package{
    struct vport* arrival_vport;
    struct sk_buff* arrival_skb;
};



#endif