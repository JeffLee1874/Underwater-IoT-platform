#include "simulation.h"

int compute(struct vport* from, struct vport* to, struct sk_buff* new){

    //can arrive perfectly
    new->dev = to->vnode->dev;
    //1000ms
    return 1000;
}

// int compute(struct vport* from, struct vport* to){
    
//     //1000ms
//     return 1000;
// }