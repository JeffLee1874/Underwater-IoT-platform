#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h> 
#include <linux/hrtimer.h>   /* hrtimer */
#include <linux/ktime.h>  
#include <linux/timekeeping.h>  
#include <linux/module.h>

#include "vmac.h"

MODULE_LICENSE("Dual BSD/GPL");

extern struct vmac_ops *mac_ptc_ops;

// void vmac_contral(struct sk_buff* skb,struct tx_hrtimer *tx_h,struct net_device *dev){
//     struct timespec time;
//     static ktime_t ktime;
//     ktime = ms_to_ktime(INTERVAL);
//     tx_h->skb = skb;
//     //hrtimer_cancel(&tx_h->hr_timer);
//     getnstimeofday(&time);
//     printk("<0>""hrtimer setting up:%9llu sec, %9lld ms, %9lld ns,send a packet\n",(unsigned long long) time.tv_sec,(unsigned long long) time.tv_sec*1000 + time.tv_nsec/1000000 ,(long long)time.tv_nsec);
//     hrtimer_start(&tx_h->hr_timer, ktime, HRTIMER_MODE_REL );
// }


int register_vmac_ops(struct vmac_ops *ops){
    if(ops->vmac_recv_fun==NULL||ops->vmac_send_fun==NULL){
        return -1;
    }
    if(ops->hdrlen<0||ops->hdrlen>100){
        ops->hdrlen = 50;
    }
    mac_ptc_ops = ops;
    return 0;
}