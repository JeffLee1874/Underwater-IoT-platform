#include "vport.h"

enum hrtimer_restart send(struct hrtimer *timer)
{
	printk("enter send func!!!!!!!!!!!!!");
    struct work *work = container_of(timer, typeof(*work), hr_timer);
    printk("to_device: %s", work->skb->dev->name);
	vport_send(work->skb, NULL);

    printk(KERN_ALERT "This line is printed after 1 seconds.\n");
  
    return HRTIMER_NORESTART;
}

void start_simulate(struct work_struct* w){
	printk("12333");
    struct work *work = container_of(w, typeof(*work), work_test);
	struct vport *vport = work->vport;
    struct sk_buff *skb = work->skb;
    
    int delay = 0;
    printk("who send this?  %s  || %s", skb->dev->name, vport->name);

    struct hlist_node *hn;
    struct vport* to_vport;
    for(hn = vport->phy_switcher->entry->first; hn!=NULL; hn = hn->next)
    {
        to_vport = container_of(hn, typeof(*to_vport), hash_node);
        if(!strcmp(to_vport->name, vport->name)){
            continue;
        }
        printk("now is %s", to_vport->name);
		
		struct work *temp = (struct work *)kmalloc(sizeof(struct work), GFP_KERNEL);
		temp->skb = skb_copy(skb, GFP_ATOMIC);
        delay = vport->phy_switcher->ops->compute(vport, to_vport, temp->skb);
        if(delay > 0)
        {
			// temp->skb = skb_copy(skb, GFP_ATOMIC);
            // temp->skb->dev = to_vport->vnode->dev;
            printk("after computation, skb->dev is %s || %s", temp->skb->dev->name, to_vport->vnode->dev->name);
            
			temp->data = "COMPLETE!";
            hrtimer_init(&temp->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
            temp->hr_timer.function = send;
            hrtimer_start(&temp->hr_timer, ms_to_ktime(delay), HRTIMER_MODE_REL);
			pid_t tid = get_current()->pid;
			printk("pid=%u  ------  delay=%u ms", tid, delay);
            // vport_send(new, to_vport);
        }
    }
    kfree(skb);
}

void vport_received(struct sk_buff *skb, struct ip_tunnel_info *tun_info)
{
    struct vport *vp = (struct vport* )rcu_dereference_rtnl(skb->dev->rx_handler_data);
	if (unlikely(!vp))
    {
        printk("-----Can't find vport-----");
        goto error;
    }
	printk("--from-- rx_handler_data --get-- vport name: %s", vp->name);

	if (unlikely(skb_warn_if_lro(skb)))
		goto error;

	/* Make our own copy of the packet.  Otherwise we will mangle the
	 * packet for anyone who came before us (e.g. tcpdump via AF_PACKET).
	 */
	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(!skb))
		return;

	if (skb->dev->type == ARPHRD_ETHER) {
		skb_push(skb, ETH_HLEN);
		skb_postpush_rcsum(skb, skb->data, ETH_HLEN);
	}
    struct work *work = (struct work*)kmalloc(sizeof(struct work), GFP_KERNEL);
	work->skb = skb;
	work->vport = vp;
	
	INIT_WORK(&work->work_test, start_simulate);
    /* 将自己的工作项添加到指定的工作队列去， 同时唤醒相应线程处理 */
    queue_work(vp->phy_switcher->ops->workqueue, &work->work_test);
	printk("123");
	return;
error:
	kfree_skb(skb);
}

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

static int packet_length(const struct sk_buff *skb,
			 struct net_device *dev)
{
	int length = skb->len - dev->hard_header_len;

	if (!skb_vlan_tag_present(skb) &&
	    eth_type_vlan(skb->protocol))
		length -= VLAN_HLEN;

	/* Don't subtract for multiple VLAN tags. Most (all?) drivers allow
	 * (ETH_LEN + VLAN_HLEN) in addition to the mtu value, but almost none
	 * account for 802.1ad. e.g. is_skb_forwardable().
	 */

	return length > 0 ? length: 0;
}

void vport_send(struct sk_buff *skb, struct vport* vport){
    // struct sk_buff* skb = package->arrival_skb;
    // struct vport* vport = package->arrival_vport;

    // int mtu = vport->vnode->dev->mtu;

	// switch (vport->vnode->dev->type) {
	// case ARPHRD_NONE:
	// 	if (mac_proto == MAC_PROTO_ETHERNET) {
	// 		skb_reset_network_header(skb);
	// 		skb_reset_mac_len(skb);
	// 		skb->protocol = htons(ETH_P_TEB);
	// 	} else if (mac_proto != MAC_PROTO_NONE) {
	// 		WARN_ON_ONCE(1);
	// 		goto drop;
	// 	}
	// 	break;
	// case ARPHRD_ETHER:
	// 	if (mac_proto != MAC_PROTO_ETHERNET)
	// 		goto drop;
	// 	break;
	// default:
	// 	goto drop;
	// }

	// if (unlikely(packet_length(skb, vport->vnode->dev) > mtu &&
	// 	     !skb_is_gso(skb))) {
	// 	net_warn_ratelimited("%s: dropped over-mtu packet: %d > %d\n",
	// 			     vport->vnode->dev->name,
	// 			     packet_length(skb, vport->vnode->dev), mtu);
	// 	vport->vnode->dev->stats.tx_errors++;
	// 	goto drop;
	// }

	// skb->dev = vport->vnode->dev;
	printk("now in SEND FUNC, to_device: %s", skb->dev->name);
	dev_queue_xmit(skb);
	return;

drop:
	kfree_skb(skb);
}

void vport_free(struct vport *vport){
	kfree(vport);
}