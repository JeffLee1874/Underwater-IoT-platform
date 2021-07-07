#include"vmac.h"
#include"mac/mac_simple.h"
#include"trace.h"

#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/sched.h>
#include <linux/kernel.h>  /* printk() */
#include <linux/slab.h>    /* kmalloc() */
#include <linux/errno.h>   /* error codes */
#include <linux/types.h>   /* size_t */
#include <linux/interrupt.h> /* mark_bh */

#include <linux/in.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include <linux/tcp.h>         /* struct tcphdr */
#include <linux/skbuff.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#include <linux/hrtimer.h>   /* hrtimer */
#include <linux/ktime.h>  
#include <linux/timekeeping.h>  



MODULE_LICENSE("Dual BSD/GPL");

struct net_device *vmac_dev;
int timeout = VMAC_TIMEOUT;

struct vmac_ops *mac_ptc_ops;


struct task_struct *task_p = NULL;
struct tx_hrtimer *tx_h; 
struct rx_hrtimer *rx_h;
struct timespec64 time;


enum hrtimer_restart tx_hrtimer_callback( struct hrtimer *timer ) {
    //u8 mac_header[50];
    struct sk_buff *skb;
    struct tx_hrtimer *t_data = container_of(timer, typeof(*t_data), hr_timer);
    struct net_device *dev_ens33 = dev_get_by_name(&init_net,VMAC_DEVICE);
    struct vmac_priv *priv = netdev_priv(vmac_dev);
    skb = t_data->skb;
    priv->skb = skb;
    priv->stats.tx_packets++;
    priv->stats.tx_bytes += skb->len;
    if(t_data->flag==0)
        send_event(skb);
    else
        resend_event(skb);
    dev_ens33->netdev_ops->ndo_start_xmit(skb,dev_ens33);
    priv->flag = 0;
    wake_up_process(task_p);
    ktime_get_ts64(&time);
    printk("<0>""hrtimer callback:%llu sec, %lld ms, %lld ns,send a packet\n",
    (unsigned long long) time.tv_sec,(unsigned long long) time.tv_sec*1000 
    + time.tv_nsec/1000000 ,(long long)time.tv_nsec);
	//memcpy(mac_header,skb->mac_header, 8); //strlen(skb->mac_header)
    return HRTIMER_NORESTART;
}

enum hrtimer_restart rx_hrtimer_callback( struct hrtimer *timer ) {
    //u8 mac_header[50];
    struct vmac_priv *priv = netdev_priv(vmac_dev);
    struct sk_buff *skb;
    struct rx_hrtimer *t_data = container_of(timer, typeof(*t_data), hr_timer);
    skb = t_data->skb;
    recv_event(skb);
    vmac_rx(skb);
    priv->rx_state = MAC_IDLE;
    return HRTIMER_NORESTART;
}




static inline struct vmac_port *vmac_port_get_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler_data);
}




int vmac_enqueue_buf(struct net_device *dev, struct sk_buff **pskb)
{
	unsigned long flags;
    struct sk_buff *h_skb;
    struct sk_buff *skb = *pskb;
	struct vmac_priv *priv = netdev_priv(dev);
	spin_lock_irqsave(&priv->lock, flags);
    int size = priv->buffer_size;
    if(size == MAX_SIZE){
        printk("<0>""max_size:%d",size);
        spin_unlock_irqrestore(&priv->lock, flags);
        return QUEUE_FULL;
    }
    struct tx_queue_head *tx_head = priv->tx_head;
    if(tx_head->head != NULL){
        h_skb = tx_head->head;
        h_skb->prev = skb;
        skb->next = h_skb;
        skb->prev = tx_head->tail;
        tx_head->tail->next = skb;
        tx_head->head = skb;
    }
    else{
        tx_head->head = skb;
        tx_head->tail = skb;
        skb->next = skb;
        skb->prev = skb;
    }
    priv->buffer_size += 1;
    enqueue_event(skb);
	spin_unlock_irqrestore(&priv->lock, flags);
    return 0;
}
 
struct sk_buff *vmac_dequeue_buf(struct net_device *dev)
{
	struct vmac_priv *priv = netdev_priv(dev);
	struct sk_buff *skb;
	unsigned long flags;
	spin_lock_irqsave(&priv->lock, flags);
    int size = priv->buffer_size;
    if(size == 0){
        return QUEUE_EMPTY;
    }
    struct tx_queue_head *tx_head = priv->tx_head;
    skb = tx_head->tail;
    if(skb->prev == skb){
        tx_head->head = NULL;
        tx_head->tail = NULL;
    }
    else{
        tx_head->tail = skb->prev;
        skb->prev->next = tx_head->head;
        tx_head->head->prev = skb->prev;
    }
    priv->buffer_size -= 1;
    dequeue_event(skb);
	spin_unlock_irqrestore(&priv->lock, flags);
	return skb;
}


int vmac_open(struct net_device *dev)
{
    /* request_region(), request_irq(), ....  (like fops->open) */

    /*
     * Assign the hardware address of the board: use "\0VMACx", where
     * x is 0 or 1. The first byte is '\0' to avoid being a multicast
     * address (the first byte of multicast addrs is odd).
     */
    /* [cgw]: 分配一个假的硬件地址，真正的网卡的时候，这个地址是从网卡读出来的 */
    memcpy(dev->dev_addr, MAC_ADDR, ETH_ALEN);
    /* [cgw]: 启动发送队列 */
    netif_start_queue(dev);
    printk("vmac_open\n");
    return 0;
}



int vmac_release(struct net_device *dev)
{
    netif_stop_queue(dev);    
 
    return 0;
}



int vmac_tx(struct sk_buff *skb, struct net_device *dev){
    struct vmac_priv *priv = netdev_priv(dev);
    struct sk_buff **pskb = &skb;
    if(vmac_enqueue_buf(dev,pskb)==QUEUE_FULL){
        printk("<0>""TX Buffer not free!\n");
        ktime_get_ts64(&time);
        printk("<0>""hrtimer:%9llu sec, %9lld ms, %9lld ns,send a packet\n",
        (unsigned long long) time.tv_sec,(unsigned long long) time.tv_sec*1000 
        + time.tv_nsec/1000000 ,(long long)time.tv_nsec);
        if(!netif_queue_stopped(dev)){
            netif_stop_queue(dev);
        }
        return NETDEV_TX_BUSY;
    }
    netdev_get_tx_queue(dev,0)->trans_start = jiffies;
    wake_up_process(task_p);
    return 0;
}



int vmac_thread(void *dev)
{
    struct vmac_priv *priv = netdev_priv(dev);
    while(1){
        struct sk_buff *skb;
        while(priv->flag!=0 && vmac_dev){
                schedule();
        }
        if(priv->tx_head->head != NULL&&(skb = vmac_dequeue_buf(dev))!=QUEUE_EMPTY){
            priv->flag = 1;
            mac_ptc_ops->vmac_send_fun(skb,tx_h,dev); 
            // wait_event_interruptible()
        }
        else{
            set_current_state(TASK_INTERRUPTIBLE);
            schedule();
        }
        set_current_state(TASK_RUNNING);
        if(kthread_should_stop()){
            break;
        }
    }
    return 0;
}


void vmac_rx( struct sk_buff *skb)
{
    struct vmac_priv *priv = netdev_priv(vmac_dev);
    if (!skb) 
    {
        printk("vmac rx: low on mem - packet dropped\n");
        priv->stats.rx_dropped++;
        return;
    }
	skb->dev = priv->dev;
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += skb->len;
	netif_receive_skb(skb);
    return;
}


void vmac_tx_timeout (struct net_device *dev， unsigned int n)
{
    struct vmac_priv *priv = netdev_priv(dev);
    printk("<0>""Transmit timeout at %ld, latency %ld\n", jiffies,jiffies - netdev_get_tx_queue(dev,0)->trans_start);
    priv->stats.tx_errors++;
    netif_wake_queue(dev);
    return;
}


int vmac_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
    return 0;
}



struct net_device_stats *vmac_stats(struct net_device *dev)
{
    struct vmac_priv *priv = netdev_priv(dev);
 
    return &priv->stats;
}


static rx_handler_result_t netdev_frame_hook(struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;
    const unsigned char *dest = eth_hdr(skb)->h_dest;

	if (unlikely(skb->pkt_type == PACKET_LOOPBACK))
		return RX_HANDLER_PASS;


    struct vmac_port *p = vmac_port_get_rcu(skb->dev);
    if (ether_addr_equal(p->vmac_dev->dev_addr, dest))
        skb->pkt_type = PACKET_HOST; 

    // printk("<0>""tstamp:%d \n",(unsigned long)skb->tstamp);
    mac_ptc_ops->vmac_recv_fun(skb,rx_h,vmac_dev);
	return RX_HANDLER_CONSUMED;
    
}



static const struct net_device_ops vmac_dev_ops = {
    .ndo_open = vmac_open,
    .ndo_stop = vmac_release,
    .ndo_start_xmit = vmac_tx,
    .ndo_tx_timeout = vmac_tx_timeout,
    .ndo_do_ioctl = vmac_ioctl,
    .ndo_get_stats = vmac_stats,
};

struct tx_queue_head tx_head = {
        .head = NULL,
        .tail = NULL,
};



void vmac_init(struct net_device *dev)
{
    struct vmac_priv *priv = NULL;
    ether_setup(dev); 
    dev->netdev_ops = &vmac_dev_ops;
    dev->watchdog_timeo = timeout;
    dev->features |= NETIF_F_IP_CSUM;
    priv = netdev_priv(dev);   
    memset(priv, 0, sizeof(struct vmac_priv));
    priv->dev = dev; 
    priv->tx_head = &tx_head;
    priv->buffer_size = 0;
    priv->flag = 0;
    priv->tx_state = MAC_IDLE;
    priv->rx_state = MAC_IDLE;
    spin_lock_init(&((struct vmac_priv *)priv)->lock);
    return;
}

struct vmac_port* vmac_port_alloc(void){
    struct vmac_port *p;
    size_t alloc_size;
    alloc_size = sizeof(struct vmac_port);
    p = kzalloc(alloc_size, GFP_KERNEL);
    p->dev = dev_get_by_name(&init_net,VMAC_DEVICE);
    return p;
}


static __exit void vmac_cleanup(void)
{
    kthread_stop(task_p);
    hrtimer_cancel(&tx_h->hr_timer);
    hrtimer_cancel(&rx_h->hr_timer);
    if(vmac_dev)
    {            
        unregister_netdev(vmac_dev);    
        free_netdev(vmac_dev);   
    }
    if(tx_h){
        kfree(tx_h);
    }
    if(rx_h){
        kfree(rx_h);
    }
    if(mac_ptc_ops){
        kfree(mac_ptc_ops);
    }    
    struct net_device *dev = dev_get_by_name(&init_net,VMAC_DEVICE);
    netdev_rx_handler_unregister(dev);
    trace_write();
    return;
}



static __init int vmac_init_module(void)
{
    trace_init();
    int result,ret = -ENOMEM;
    printk("<0>""VMac is not loaded yet");
	vmac_dev = alloc_netdev(sizeof(struct vmac_priv), "vmac", NET_NAME_UNKNOWN, vmac_init);
	if (vmac_dev == NULL)
		goto out;
 
    ret = -ENODEV;
	if ((result = register_netdev(vmac_dev)))
		printk("vmac: error %i registering device \"%s\"\n",
				result, vmac_dev->name);
	else
		ret = 0;
    
    if(netdev_priv(vmac_dev)!=NULL){
        struct vmac_port *p = vmac_port_alloc();
        p->vmac_dev = vmac_dev;
        netdev_rx_handler_register(p->dev,netdev_frame_hook,p);
    }
    struct net_device *dev = dev_get_by_name(&init_net,VMAC_DEVICE);
    dev->flags |= IFF_PROMISC;
    dev->gflags |= IFF_PROMISC;

    //hrtimer
    tx_h =  (struct tx_hrtimer*)kmalloc(sizeof(struct tx_hrtimer), GFP_KERNEL);
    hrtimer_init(&tx_h->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    tx_h->hr_timer.function = tx_hrtimer_callback;

    rx_h =  (struct rx_hrtimer*)kmalloc(sizeof(struct rx_hrtimer), GFP_KERNEL);
    hrtimer_init(&rx_h->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    rx_h->hr_timer.function = rx_hrtimer_callback;

    init_mac(); //init mac-simple ops

    //kthread
    task_p = kthread_create(vmac_thread,vmac_dev,"vmac task");
    if(IS_ERR(task_p))
        goto out;
        
    printk("<0>""vmac is loading");
    return ret;
    out:
        vmac_cleanup();
	    return ret;
}

    // rtnl_lock();
    // rtnl_unlock();


module_init(vmac_init_module);
module_exit(vmac_cleanup);