#ifndef _TYPEDEF_H
#define _TYPEDEF_H
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/dst_metadata.h>
#include <net/net_namespace.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/hrtimer.h>

struct simulation_package{
    struct vport* arrival_vport;
    struct sk_buff* arrival_skb;
};

struct work{
  struct work_struct work_test;
  struct hrtimer hr_timer;
//   struct simulation_package *sp;
  struct sk_buff *skb;
  struct vport *vport;
  char* index;
  char* data;
};

struct simulation_ops
{
    struct workqueue_struct *workqueue;
    int (*compute)(struct vport* from, struct vport* to, struct sk_buff* new);
    // int (*compute)(struct vport* from, struct vport* to);
};

/**
 * @switcher_name: phy_switcher name
 * @entry: hash list entry
 */
struct phy_switcher
{
    char *switcher_name;
    struct hlist_head *entry;
    struct hlist_node *h_node;
    struct simulation_ops *ops;
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
	// struct rcu_head *rcu;
    struct vnode *vnode;
};



#endif