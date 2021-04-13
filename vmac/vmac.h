#ifndef __VMAC_H__
#define __VMAC_H__

#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/netdevice.h> 
#include <linux/fs.h>

#define MAX_SIZE 20
#define QUEUE_FULL -1
#define QUEUE_EMPTY -2
#define VMAC_TIMEOUT 20
#define ARPHRD_ETHER 	1
#define ARPHRD_NONE	  0xFFFE

#define MAC_IDLE	  0x0000  //空闲
#define	MAC_POLLING	  0x0001  //轮询
#define	MAC_RECV 	  0x0010  //接收
#define	MAC_SEND 	  0x0100  //发送
#define	MAC_RTS		  0x0200
#define	MAC_BCN		  0x0300
#define	MAC_CTS		  0x0400
#define	MAC_ACK		  0x0800
#define	MAC_COLL	  0x1000  //碰撞
#define	MAC_MGMT	  0x1001

#define INTERVAL 5    //delay
#define MAC_ADDR "\0VMAC0"
#define VMAC_DEVICE "ens33"

#define BANDWIDTH 10000000

struct tx_hrtimer;

struct rx_hrtimer;

typedef void (*vmac_send)(struct sk_buff* skb,struct tx_hrtimer *tx_h,struct net_device *dev); // send_func

typedef int (*vmac_recv)(struct sk_buff* skb,struct rx_hrtimer *rx_h,struct net_device *dev); //recv_func

//static const unsigned long INTERVAL= 5;  



struct tx_queue_head {
    struct sk_buff *head;
    struct sk_buff *tail;
};

struct tx_hrtimer{  
    struct hrtimer hr_timer;  
    struct sk_buff *skb;
    int flag;  
};

struct rx_hrtimer{  
    struct hrtimer hr_timer;  
    struct sk_buff *skb;  
};


struct vmac_priv {
    struct net_device_stats stats;
    struct tx_queue_head *tx_head;  /* List of input packets */
    int buffer_size;
    int tx_packetlen;
    u8 *tx_packetdata;
    struct sk_buff *skb;
    spinlock_t lock;
    int *flag;
    struct net_device *dev;
    int tx_state;
    int rx_state;
};

struct vmac_port{
    struct net_device *dev;
    struct net_device *vmac_dev;
};


struct vmac_ops{
    //unsigned long bandwidth;
    int hdrlen;
    vmac_send vmac_send_fun;
    vmac_recv vmac_recv_fun;
};

void vmac_rx( struct sk_buff *skb);
struct sk_buff *vmac_dequeue_buf(struct net_device *dev);
int vmac_enqueue_buf(struct net_device *dev, struct sk_buff **pskb);
struct vmac_port* vmac_port_alloc(void);

void vmac_contral(struct sk_buff* skb,struct tx_hrtimer *m_h,struct net_device *dev);

int register_vmac_ops(struct vmac_ops *ops);

void init_mac(void);

#endif