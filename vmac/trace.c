#include "trace.h"

struct file *trace;
mm_segment_t fs;
loff_t pos;

struct trace_head *tx_list_head;  //init()
struct trace_head *rx_list_head; //init()
struct trace_head *tq_list_head; //init()

int tx_n;  //init()
int rx_n;  //init()
int tq_n;  //init() 

void refresh_cache(struct trace_head *list_head);

void mac_transf_16(unsigned char *addr);

void trace_init(){
    tx_list_head = kzalloc(sizeof(struct trace_head),GFP_KERNEL);
    rx_list_head = kzalloc(sizeof(struct trace_head),GFP_KERNEL);
    tq_list_head = kzalloc(sizeof(struct trace_head),GFP_KERNEL);
    tx_list_head->head = NULL;
    tx_list_head->tail = NULL;
    rx_list_head->head = NULL;
    rx_list_head->tail = NULL;
    tq_list_head->head = NULL;
    tq_list_head->tail = NULL;
    tx_n = 0;
    rx_n = 0;
    tq_n = 0;
    trace = NULL;
    pos = 0;
}

void send_event(struct sk_buff *skb){
    if(tx_n<10000){
        struct timespec64 time;
        ktime_get_ts64(&time);
        struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
        struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
        trace->ip_src = (unsigned int)ip_header->saddr;
        trace->ip_dest = (unsigned int)ip_header->daddr;
        trace->no = tx_n;
        trace->event = SEND;
        trace->src = eth_hdr(skb)->h_source;
        trace->dest = eth_hdr(skb)->h_dest;
        trace->tstamp = time.tv_nsec;
        trace->len = skb->len;
        if(tx_list_head->head==NULL){
            tx_list_head->head = trace;
            tx_list_head->tail = trace;
            trace->next = NULL;
        }else{
            tx_list_head->tail->next = trace;
            tx_list_head->tail = trace;
            trace->next = NULL;
        }
    }
    tx_n++;
}


void resend_event(struct sk_buff *skb){
    if(tx_n<10000){
        struct timespec64 time;
        ktime_get_ts64(&time);
        struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
        struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
        trace->ip_src = (unsigned int)ip_header->saddr;
        trace->ip_dest = (unsigned int)ip_header->daddr;
        trace->no = tx_n;
        trace->event = RESEND;
        trace->src = eth_hdr(skb)->h_source;
        trace->dest = eth_hdr(skb)->h_dest;
        trace->tstamp = time.tv_nsec;
        trace->len = skb->len;    
        if(tx_list_head->head==NULL){
            tx_list_head->head = trace;
            tx_list_head->tail = trace;
            trace->next = NULL;
        }else{
            tx_list_head->tail->next = trace;
            tx_list_head->tail = trace;
            trace->next = NULL;
        }
    }
    tx_n++;
}


void recv_event(struct sk_buff *skb){
    if(rx_n<10000){
        struct timespec64 time;
        ktime_get_ts64(&time);
        struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
        struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
        trace->ip_src = (unsigned int)ip_header->saddr;
        trace->ip_dest = (unsigned int)ip_header->daddr;
        trace->no = rx_n;
        trace->event = RECV;
        trace->src = eth_hdr(skb)->h_source;
        trace->dest = eth_hdr(skb)->h_dest;
        trace->tstamp = time.tv_nsec;
        trace->len = skb->len;
        if(rx_list_head->head==NULL){
            rx_list_head->head = trace;
            rx_list_head->tail = trace;
            trace->next = NULL;
        }else{
            rx_list_head->tail->next = trace;
            rx_list_head->tail = trace;
            trace->next = NULL;
        }
    }
    rx_n++;
}

void enqueue_event(struct sk_buff *skb){
    if(tq_n<10000){
        struct timespec64 time;
        ktime_get_ts64(&time);
        struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
        struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
        trace->ip_src = (unsigned int)ip_header->saddr;
        trace->ip_dest = (unsigned int)ip_header->daddr;
        trace->no = rx_n;
        trace->event = ENQUEUE;
        trace->src = eth_hdr(skb)->h_source;
        trace->dest = eth_hdr(skb)->h_dest;
        trace->tstamp = time.tv_nsec;
        trace->len = skb->len;
        if(tq_list_head->head==NULL){
            tq_list_head->head = trace;
            tq_list_head->tail = trace;
            trace->next = NULL;
        }else{
            tq_list_head->tail->next = trace;
            tq_list_head->tail = trace;
            trace->next = NULL;
        }
    }
    tq_n++;
}


void dequeue_event(struct sk_buff *skb){
    if(tq_n<10000){
        struct timespec64 time;
        ktime_get_ts64(&time);
        struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);
        struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
        trace->ip_src = (unsigned int)ip_header->saddr;
        trace->ip_dest = (unsigned int)ip_header->daddr;
        trace->no = rx_n;
        trace->event = DEQUEUE;
        trace->src = eth_hdr(skb)->h_source;
        trace->dest = eth_hdr(skb)->h_dest;
        trace->tstamp = time.tv_nsec;
        trace->len = skb->len;
        if(tq_list_head->head==NULL){
            tq_list_head->head = trace;
            tq_list_head->tail = trace;
            trace->next = NULL;
        }else{
            tq_list_head->tail->next = trace;
            tq_list_head->tail = trace;
            trace->next = NULL;
        }
    }
    tq_n++;
}


// void loss_event(struct sk_buff *skb){
//     if(rx_n<10000){
//         struct timespec64 time;
//         ktime_get_ts64(&time);
//         struct trace_list *trace = kzalloc(sizeof(struct trace_list), GFP_KERNEL);
//         trace->no = rx_n;
//         trace->event = LOSS;
//         trace->src = eth_hdr(skb)->h_source;
//         trace->dest = eth_hdr(skb)->h_dest;
//         trace->tstamp = time.tv_nsec;
//         trace->len = skb->len;
//         if(rx_list_head->head==NULL){
//             rx_list_head->head = trace;
//             rx_list_head->tail = trace;
//             trace->next = NULL;
//         }else{
//             rx_list_head->tail->next = trace;
//             rx_list_head->tail = trace;
//             trace->next = NULL;
//         }
//     }
//     rx_n++;
// }


void trace_write(){
    trace =filp_open(MY_FILE,O_RDWR|O_CREAT,0777);
    if (IS_ERR(trace))
        printk("create file error\n");
    fs = get_fs();
    set_fs(KERNEL_DS);
    char tx_trace[75] = "TX TRACE:\nNO   TIME   EVENT   MAC_SRC   MAC_DEST   IP_SRC   IP_DEST   LEN\n";
    vfs_write(trace,tx_trace, sizeof(tx_trace)-1, &pos);
    refresh_cache(tx_list_head);
    char tq_trace[75] = "TX_QUEUE:\nNO   TIME   EVENT   MAC_SRC   MAC_DEST   IP_SRC   IP_DEST   LEN\n";
    vfs_write(trace,tq_trace, sizeof(tq_trace)-1, &pos);
    refresh_cache(tq_list_head);
    char rx_trace[75] = "RX TRACE:\nNO   TIME   EVENT   MAC_SRC   MAC_DEST   IP_SRC   IP_DEST   LEN\n";
    vfs_write(trace,rx_trace, sizeof(rx_trace)-1, &pos);
    refresh_cache(rx_list_head);
    filp_close(trace,NULL);
    set_fs(fs);
}


void refresh_cache(struct trace_head *list_head){
    while(list_head->head!=NULL){
        int i = 0;
        char buf[120] = "";
        struct trace_list *element= list_head->head;
        char event[8];
        if(element->event == SEND)
            sprintf(event,"%s","SEND   ");
        if(element->event == RESEND)
            sprintf(event,"%s","RESEND ");
        if(element->event == RECV)
            sprintf(event,"%s","RECV   ");
        if(element->event == LOSS)
            sprintf(event,"%s","LOSS   ");
        if(element->event == ENQUEUE)
            sprintf(event,"%s","ENQUEUE");
        if(element->event == DEQUEUE)
            sprintf(event,"%s","DEQUEUE");
        // mac_transf_16(element->src);
        // mac_transf_16(element->dest);
        sprintf(buf,"%05d   %0llu   %s   "MAC_FMT"   "MAC_FMT"   %pI4   %pI4   %d\n",
        element->no,element->tstamp,event,MAC_ARG(element->src),MAC_ARG(element->dest),
        &element->ip_src,&element->ip_dest,element->len);
        while(buf[i]!='\n'){
            i++;
        }
        vfs_write(trace,buf, i+1, &pos);
        list_head->head = element->next;
        kfree(element);
    }
    kfree(list_head);
}
