#include "vport.h"


void vport_received(struct sk_buff *skb, struct ip_tunnel_info *tun_info)
{
	// vport = netdev_get_vport(skb->dev);
	// struct net_device **device = &skb->dev;
    struct vport *vp = (struct vport* )rcu_dereference_rtnl(skb->dev->rx_handler_data);
	// search_vport_by_name(,sk->dev->name,vp);
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
	
	start_simulate(vp, skb);
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
	dev_queue_xmit(skb);
	return;

drop:
	kfree_skb(skb);
}

void vport_free(struct vport *vport){
	kfree(vport);
}