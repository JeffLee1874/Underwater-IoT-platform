#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <linux/skbuff.h>
#include "../typedef.h"
#include "../vport.h"
#include "../phy_switcher.h"
#include <linux/slab.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>


int compute(struct vport* from, struct vport* to);

#endif