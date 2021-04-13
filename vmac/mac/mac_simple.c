#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h> 
#include <linux/hrtimer.h>   /* hrtimer */
#include <linux/ktime.h>  
#include <linux/timekeeping.h> 

#include "mac_simple.h"



unsigned long bandwidth = BANDWIDTH;  //10M

struct sk_buff *skb_tx = NULL;
struct sk_buff *skb_rx = NULL;

unsigned long skb_tx_time = 0;
unsigned long skb_rx_time = 0;

struct timespec64 stime;


unsigned long txtime(struct sk_buff* skb,unsigned long bandwidth){ //return us
    unsigned long len = (unsigned long)skb->len;
    unsigned long t = 8000000*len/bandwidth;  
    return t;  //us
}

unsigned long expire(void){
    struct timespec64 ctime;
    unsigned long t = 0;
    ktime_get_ts64(&ctime);
    if(((unsigned long)((ctime.tv_nsec-stime.tv_nsec)/1000))<skb_rx_time){
        t = (skb_rx_time - (unsigned long)((ctime.tv_nsec-stime.tv_nsec)/1000));
    }
    return t; //us
}

unsigned long random(void){
    struct timespec64 time;
    ktime_get_ts64(&time);
    unsigned long random = 100*(time.tv_nsec%100)*1000000/bandwidth;
    return random;
}

int mac_recv(struct sk_buff* skb,struct rx_hrtimer *rx_h,struct net_device *dev){
    struct vmac_priv *priv = netdev_priv(dev);
    if(priv->rx_state == MAC_IDLE){
        priv->rx_state = MAC_RECV; //set recv
        skb_rx = skb;
        skb_rx_time = txtime(skb,bandwidth);
        ktime_t ktime = ns_to_ktime(skb_rx_time*1000);
        rx_h->skb = skb;
        hrtimer_start(&rx_h->hr_timer, ktime, HRTIMER_MODE_REL );
        ktime_get_ts64(&stime);
    }else{
        priv->rx_state = MAC_COLL; //set coll
        unsigned long time = txtime(skb,bandwidth);
        if(time>expire()&& expire()!=0 ){
            hrtimer_cancel(&rx_h->hr_timer);
            if(skb_rx!=NULL&&priv->rx_state!=MAC_IDLE){ //这里必须再对接收状态做一次判断
                dev_kfree_skb(skb_rx);
            }
            skb_rx = skb;
            rx_h->skb = skb;
            skb_rx_time = time;
            ktime_t ktime = ns_to_ktime(skb_rx_time*1000);
            ktime_get_ts64(&stime);
            hrtimer_start(&rx_h->hr_timer, ktime, HRTIMER_MODE_REL);
            printk("<0>""RX_COLL \n");
        }else{
            dev_kfree_skb(skb);
        }
    }
    return 0;
}


void mac_send(struct sk_buff* skb,struct tx_hrtimer *tx_h,struct net_device *dev){
    struct vmac_priv *priv = netdev_priv(dev);
    if(priv->rx_state == MAC_IDLE){
        skb_tx_time = txtime(skb,bandwidth);
        ktime_t ktime = ns_to_ktime((skb_tx_time+random())*1000);
        tx_h->skb = skb;
        tx_h->flag = 0;
        hrtimer_start(&tx_h->hr_timer, ktime, HRTIMER_MODE_REL );
    }else{
        skb_tx_time = txtime(skb,bandwidth);
        ktime_t ktime= ns_to_ktime((skb_tx_time+random()+expire())*1000);
        tx_h->skb = skb;
        tx_h->flag = 1;
        hrtimer_start(&tx_h->hr_timer, ktime, HRTIMER_MODE_REL );
        printk("<0>""TX_COLL \n");
    }
}


void init_mac(void){  // 如何初始化由mac-simple协议决定
    struct vmac_ops *ops = kzalloc(sizeof(struct vmac_ops), GFP_KERNEL);
    ops->hdrlen = 0;
    ops->vmac_send_fun = mac_send;
    ops->vmac_recv_fun = mac_recv;
    register_vmac_ops(ops);
}