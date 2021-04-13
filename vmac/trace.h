#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>   /* struct device, and other headers */
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>          /* struct iphdr */
#include<linux/if_ether.h>

#ifndef __TRACE_H__
#define __TRACE_H__

#define MAC_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define MAC_ARG(x)  ((u8*)(x))[0],((u8*)(x))[1],((u8*)(x))[2],((u8*)(x))[3],((u8*)(x))[4],((u8*)(x))[5]

#define MY_FILE "./trace.txt"
#define SEND 0
#define RESEND 1
#define RECV 2
#define LOSS 3
#define DEQUEUE 4
#define ENQUEUE 5

struct trace_head{
    struct trace_list *head;
    struct trace_list *tail;
};


struct trace_list{
    struct trace_list *next;
    int no;
    int event;
    unsigned char *src;
    unsigned char *dest;
    unsigned int ip_src;
    unsigned int ip_dest;
    unsigned long tstamp;
    int len;
};



void send_event(struct sk_buff *skb);

void recv_event(struct sk_buff *skb);

void resend_event(struct sk_buff *skb);

//void loss_event(struct sk_buff *skb);

void enqueue_event(struct sk_buff *skb);

void dequeue_event(struct sk_buff *skb);

void trace_init(void);

void trace_write(void);


#endif