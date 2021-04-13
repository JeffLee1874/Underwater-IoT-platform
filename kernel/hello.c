#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/kthread.h>

#include "typedef.h"
#include "simulation/simulation.h"
#include "vnode.h"
#include "vport.h"
#include "phy_switcher.h"

MODULE_LICENSE("GPL");

struct simulation_ops *ops;
struct phy_switcher* ps;


static int test_init(void)
{
    printk("Hello,world!\n");

    ops = (struct simulation_ops*)kmalloc(sizeof(struct simulation_ops), GFP_KERNEL);
    ops->compute = compute;
    ops->workqueue = alloc_workqueue("workqueue_test", WQ_UNBOUND, 10);

    ps = (struct phy_switcher*)kmalloc(sizeof(struct phy_switcher), GFP_KERNEL);
    if(!initialize_ps(ps, "UIOT", 8)){
        printk("----can't init phy_switcher----");
        return 0;
    }
    printk("phy_switcher %s initialized!", ps->switcher_name);
    set_simulation_option(ops, ps);


    struct vport *vp, *vp2;
    struct vnode *vn, *vn2;
    vn = (struct vnode*)kmalloc(sizeof(struct vnode), GFP_KERNEL);
    vn2 = (struct vnode*)kmalloc(sizeof(struct vnode), GFP_KERNEL);


    vnode_register(vn, "tap0", ps, vp);
    vnode_register(vn2, "tap1", ps, vp2);
    
    return 0;
}

static void test_exit(void)
{
    printk("Goodbye,cruel world!\n");
    unregister_all(ps);
}

module_init(test_init);
module_exit(test_exit);
MODULE_AUTHOR("Lee");
MODULE_DESCRIPTION("UIOT kernel module");