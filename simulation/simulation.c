#include "simulation.h"

void start_simulate(struct vport* vport, struct sk_buff *skb){
    struct simulation_package *package = (struct simulation_package *)kmalloc(sizeof(struct simulation_package), GFP_KERNEL);
    if(!package)
    {
        kfree(package);
        printk("simulation_package malloc failure!");
    }
    package->arrival_skb = skb;
    package->arrival_vport = vport;
    receive(package);

    compute(package);
    // kfree(package);
    // send(package);
}

void send(struct simulation_package *package){
    printk("---send to vport:%s || dev %s", package->arrival_vport->name, package->arrival_skb->dev->name);
    vport_send(package->arrival_skb, package->arrival_vport);
}

int compute(struct simulation_package *package){
    // struct sk_buff* skb = package->arrival_skb;
    // struct vport* vport = package->arrival_vport;
    int err = 0;
    printk("who send this?  %s  || %s", package->arrival_skb->dev->name, package->arrival_vport->name);
    if(!strcmp(package->arrival_vport->name, "tap0"))
    {
        // err = search_vport_by_name(package->arrival_vport->phy_switcher, "tap1", package->arrival_vport);
        err = search_and_send(package->arrival_vport->phy_switcher, "tap1", package->arrival_vport, package->arrival_skb);
        if(!err){
            printk("tap1 not found");
            return 0;
        }
        
        // printk("through searching, now vport is: %s || dev is: %s", package->arrival_vport->name, package->arrival_vport->vnode->dev->name);
        // package->arrival_skb->dev = package->arrival_vport->vnode->dev;
    }

    if(!strcmp(package->arrival_vport->name, "tap1"))
    {
        // err = search_vport_by_name(package->arrival_vport->phy_switcher, "tap0", package->arrival_vport);
        err = search_and_send(package->arrival_vport->phy_switcher, "tap0", package->arrival_vport, package->arrival_skb);
        if(!err){
            printk("tap0 not found");
            return 0;
        }
        // printk("through searching, now vport is: %s || dev is: %s", package->arrival_vport->name, package->arrival_vport->vnode->dev->name);
        // package->arrival_skb->dev = package->arrival_vport->vnode->dev;
    }

    return 1;
}

void receive(struct simulation_package *package){

}

void package_free(struct simulation_package *package){
    kfree(package);
}