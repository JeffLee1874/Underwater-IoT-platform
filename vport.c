#include "vport.h"

#include <linux/slab.h>

struct vport* vport_create(struct phy_switcher *phy_switcher){
    struct vport* vp = (struct vport*)kmalloc(sizeof(struct vport), GFP_KERNEL);

    if(!vp){
        vport_free(vp);
        return NULL;
    }

    vp->phy_switcher = phy_switcher;

    return vp;
}

struct vport* vport_create_ps(struct phy_switcher *phy_switcher){
        struct vport* vp = (struct vport*)kmalloc(sizeof(struct vport), GFP_KERNEL);

    if(!vp){
        vport_free(vp);
        return NULL;
    }

    vp->phy_switcher = phy_switcher;

    return vp;
}

void vport_free(struct vport *vport){
	kfree(vport);
}