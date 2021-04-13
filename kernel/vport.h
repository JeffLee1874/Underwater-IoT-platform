#ifndef _VPORT_H
#define _VPORT_H

#include <linux/types.h>
#include <linux/rculist.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <net/ip_tunnels.h>
#include <linux/rtnetlink.h>
#include <linux/slab.h>
#include <linux/kthread.h>

#include "typedef.h"
#include "phy_switcher.h"
#include "vnode.h"
// #include "simulation/simulation.h"


void start_simulate(struct work_struct* w);
// void send(struct simulation_package *package);
enum hrtimer_restart send(struct hrtimer *timer);

void vport_received(struct sk_buff *skb, struct ip_tunnel_info *tun_info);
// void vport_send(struct simulation_package *package);
void vport_send(struct sk_buff *skb, struct vport* vport);

//vport create and register to default phy_swithcer
struct vport* vport_create(struct phy_switcher *phy_switcher);
//vport create and register to chosen phy_swithcer
struct vport* vport_create_ps(struct phy_switcher *phy_switcher);
void vport_free(struct vport *vport);

#endif