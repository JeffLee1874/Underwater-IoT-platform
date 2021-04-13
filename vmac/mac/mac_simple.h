#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h> 
#include <linux/hrtimer.h>   /* hrtimer */
#include <linux/ktime.h>  
#include <linux/timekeeping.h> 

#include "../vmac.h"
#include "../trace.h"




void mac_send(struct sk_buff* skb,struct tx_hrtimer *tx_h,struct net_device *dev);

int mac_recv(struct sk_buff* skb,struct rx_hrtimer *rx_h,struct net_device *dev);

unsigned long txtime(struct sk_buff* skb,unsigned long bandwidth);

