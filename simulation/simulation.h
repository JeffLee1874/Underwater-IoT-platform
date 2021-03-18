#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <linux/skbuff.h>
#include "../typedef.h"
#include "../vport.h"
#include "../phy_switcher.h"
#include <linux/slab.h>


void start_simulate(struct vport* vport, struct sk_buff *skb);
void send(struct simulation_package *package);
int compute(struct simulation_package *package);
void receive(struct simulation_package *package);
void package_free(struct simulation_package *package);

#endif