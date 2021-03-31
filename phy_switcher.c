#include "phy_switcher.h"

#include <linux/module.h>
#include <linux/slab.h>

// #include <linux/hashtable.h> // hashtable API
#include <linux/string.h>    // strcpy, strcmp

u32 hash_name(const char *s){
    u32 key = 0;
    char c;

    while ((c = *s++))
        key += c;

    return key;
}

int initialize_ps(struct phy_switcher* ps, char* name, int size){
    if(!ps)
    {
        printk("phy_switcher malloc failure!");
        kfree(ps);
        return 0;
    }

    ps->switcher_name = name;
    // DECLARE_HASHTABLE(table, size);
    // hash_init(table);

    ps->entry = (struct hlist_head* )kmalloc(sizeof(struct hlist_head), GFP_KERNEL);
    INIT_HLIST_HEAD(ps->entry);
    
    return 1;
}

int set_simulation_option(struct simulation_ops* ops, struct phy_switcher* ps){
    if(!ops)
    {
        printk("simulation_ops is NULL!");
        return 0;
    }
    if(!ps)
    {
        printk("phy_switcher is NULL!");
        return 0;
    }
    ps->ops = ops;
    return 1;
}

int search_and_send(struct phy_switcher* ps, char* name, struct vport* vport, struct sk_buff* skb){
    int err = 0;
    // u32 key = hash_name(name);
    printk("search in phy_switcher: %s", ps->switcher_name);
    struct hlist_node *hn;
    // struct vport* vp;
    for(hn = ps->entry->first; hn!=NULL; hn = hn->next)
    {
        vport = container_of(hn, typeof(*vport), hash_node);
        printk("now is %s || search for %s", vport->name, name);
        if(!strcmp(vport->name, name))
        {
            // *vport = *vp;
            skb->dev = vport->vnode->dev;
            printk("find vport! skb->name: %s || vport->name: %s", skb->dev->name, vport->name);
            vport_send(skb, vport);
            err = 1;
            break;
        }
    }
    printk("for loop finished AND vport is: %s", vport->name);

    // hash_for_each_possible(ps->entry, vp, hash_node, key) {
    //     printk("myhashtable: match for key %u: name = %s\n",
    //         key, vp->name);

    //     // Check the name.
    //     if (!strcmp(vp->name, name)) {
    //         printk("myhashtable: element named \"foo\" found!\n");
    //         return vp;
    //     }
    // }

    
    return err;
}

int search_vport_by_name(struct phy_switcher* ps, char* name, struct vport* vport)
{
    int err = 0;
    // u32 key = hash_name(name);
    printk("search in phy_switcher: %s", ps->switcher_name);
    struct hlist_node *hn;
    // struct vport* vp;
    for(hn = ps->entry->first; hn!=NULL; hn = hn->next)
    {
        vport = container_of(hn, typeof(*vport), hash_node);
        printk("now is %s || search for %s", vport->name, name);
        if(!strcmp(vport->name, name))
        {
            // *vport = *vp;
            err = 1;
            break;
        }
    }
    printk("for loop finished AND vport is: %s", vport->name);

    // hash_for_each_possible(ps->entry, vp, hash_node, key) {
    //     printk("myhashtable: match for key %u: name = %s\n",
    //         key, vp->name);

    //     // Check the name.
    //     if (!strcmp(vp->name, name)) {
    //         printk("myhashtable: element named \"foo\" found!\n");
    //         return vp;
    //     }
    // }

    
    return err;
}

int forward_device_create(char* forward_dev_name)
{
    struct phy_switcher ps;

    // initialize the forward device
    ps.switcher_name = forward_dev_name;
    // ps.entry = (struct list_head*)kmalloc(sizeof(struct list_head), GFP_KERNEL);
    // struct work *w = container_of(work, typeof(*w), work_test);
    return 1;
}

int unregister_all(struct phy_switcher* ps){
    int err = 0;
    printk("search in phy_switcher: %s", ps->switcher_name);
    struct hlist_node *hn;
    // struct vport* vp;
    for(hn = ps->entry->first; hn!=NULL; hn = hn->next)
    {
        struct vport* vport = container_of(hn, typeof(*vport), hash_node);
        netdev_rx_handler_unregister(vport->vnode->dev);
    }
    printk("unregister finished!");
    return 1;
}