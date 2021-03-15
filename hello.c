// #include <linux/kernel.h>
// #include <linux/module.h>
// #include <linux/jiffies.h>
// #include <linux/timer.h>
// #include <linux/slab.h>
// #include <linux/timekeeping.h>

// MODULE_LICENSE("GPL");

// struct timer_data {
//     struct timer_list timer;
//     char *data;
// };

// struct timespec start;
// struct timespec end;
// struct timer_data *mytimer;

// long diff_tv(struct timespec start, struct timespec end) 
// {
//     return (end.tv_nsec - start.tv_nsec);
// }

// void my_timer_callback(struct timer_list *t) {
//   struct timer_data *t_d = from_timer(t_d, t, timer);
//   end = current_kernel_time();

//   printk(KERN_ALERT "This line is printed after 5 seconds.\n");
//   printk(KERN_ALERT "And the data is: %s \n", t_d->data);

//   printk(KERN_INFO"timer_list:%9llu sec, %9ld ns, interval_delay=%9lu ns\n", 
//         (unsigned long long) end.tv_sec, 
//         end.tv_nsec,
//         diff_tv(end, start));
// }

// static int init_module_with_timer(void) {
//   printk(KERN_ALERT "Initializing a module with timer.\n");
//   mytimer = (struct timer_data*)kmalloc(sizeof(struct timer_data), GFP_KERNEL);
//   mytimer->data = (char*)kmalloc(sizeof(char), GFP_KERNEL);
//   mytimer->data = "this is DATA field!!!!";
//   /* Setup the timer for initial use. Look in linux/timer.h for this function */
//   timer_setup(&mytimer->timer, my_timer_callback, 0);
//   mod_timer(&mytimer->timer, jiffies + msecs_to_jiffies(5000));
//   start = current_kernel_time();
//   printk(KERN_INFO "timer_list:%9llu sec, %9ld ns\n", 
//         (unsigned long long) start.tv_sec, start.tv_nsec ); 
//   return 0;
// }

// static void exit_module_with_timer(void) {
//   printk(KERN_ALERT "Goodbye, cruel world!\n");
//   del_timer(&mytimer->timer);
//   kfree(mytimer);
// }

// module_init(init_module_with_timer);
// module_exit(exit_module_with_timer);
// MODULE_AUTHOR("Lee");
// MODULE_DESCRIPTION("timer kernel module");

// #include <linux/kernel.h>
// #include <linux/module.h>
// #include <linux/hrtimer.h>
// #include <linux/ktime.h>
// #include <linux/timekeeping.h>

// // #include <linux/timer.h>
// // #include <linux/build_bug.h>
// #include <linux/slab.h>


// MODULE_LICENSE("GPL");

// struct my_hrtimer{
//   struct hrtimer hr_timer;
//   char* data;
//   // int index;
// };
// static unsigned long interval= 5000; /* unit: ms */
// struct timespec64 start;
// struct timespec64 end;
// struct my_hrtimer *m_h;
// int size = 80000;
// struct my_hrtimer* hr_timers;
// int count = 0;
// int i = 0;
// int j = 0;

// unsigned long long diff_tv(struct timespec64 start, struct timespec64 end) 
// {
//     return (end.tv_nsec - start.tv_nsec);
// }

// enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
// {
//     struct my_hrtimer *t_data = container_of(timer, typeof(*t_data), hr_timer);

//     // printk(KERN_INFO"hrtimer:%9lu sec, %9lu ns, interval_delay=%lu us\n", 
//     //         (unsigned long) uptime.tv_sec, 
//     //         uptime.tv_nsec,
//     //         (unsigned long)(diff_tv(uptimeLast, uptime))/1000); 

//   printk(KERN_ALERT "This line is printed after 5 seconds.\n");
//   printk(KERN_ALERT "And the data is: %s \n", t_data->data);

//   ktime_get_ts64(&end);
//   printk(KERN_INFO"hrtimer:%9llu sec, %9lld ns, interval_delay=%9llu ns\n", 
//           (unsigned long long) end.tv_sec, 
//           (long long)end.tv_nsec,
//           (unsigned long long)(diff_tv(end, start)));
//   // hrtimer_forward_now(timer, ms_to_ktime(interval));
 
//   return HRTIMER_NORESTART;
// }

// enum hrtimer_restart limit( struct hrtimer *timer )
// {
//   // struct my_hrtimer *t_data = container_of(timer, typeof(*t_data), hr_timer);

//     // printk(KERN_INFO"hrtimer:%9lu sec, %9lu ns, interval_delay=%lu us\n", 
//     //         (unsigned long) uptime.tv_sec, 
//     //         uptime.tv_nsec,
//     //         (unsigned long)(diff_tv(uptimeLast, uptime))/1000); 
  
//   printk(KERN_INFO"index: %d \n", count);
//   count++;

//   // printk(KERN_ALERT "This line is printed after 5 seconds.\n");
//   // printk(KERN_ALERT "And the data is: %s \n", t_data->data);

//   // ktime_get_ts64(&end);
//   // printk(KERN_INFO"hrtimer:%9llu sec, %9lld ns, interval_delay=%9llu ns\n", 
//   //         (unsigned long long) end.tv_sec, 
//   //         (long long)end.tv_nsec,
//   //         (unsigned long long)(diff_tv(end, start)));
//   // hrtimer_forward_now(timer, ms_to_ktime(interval));
 
//   return HRTIMER_NORESTART;
// }

// static int __init module_hrtimer_init( void )
// {

//     static ktime_t ktime;
//     hr_timers = (struct my_hrtimer*)kmalloc_array(size, sizeof(struct my_hrtimer), GFP_KERNEL);

//     if (hr_timers != NULL)
//     {
//       for (; i < size; i++)
//       {
//         hrtimer_init(&hr_timers[i].hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
//         hr_timers[i].hr_timer.function = limit;
//       }

//       i = 0;
//       for (; i < size; i++)
//       {
//         ktime = ms_to_ktime(interval);
//         hrtimer_start(&hr_timers[i].hr_timer, ktime, HRTIMER_MODE_REL );
//       }
//     }

//     else
//     {
//       printk(KERN_ALERT "Malloc failure\n");
//     }
    

//     m_h =  (struct my_hrtimer*)kmalloc(sizeof(struct my_hrtimer), GFP_KERNEL);
//     m_h->data = (char*)kmalloc(sizeof(char), GFP_KERNEL);
//     m_h->data = "this is DATA field!!!!";

//     printk(KERN_INFO"HR Timer module installing\n");
 
//     hrtimer_init(&m_h->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
 
//     ktime = ms_to_ktime(interval*2);
 
//     m_h->hr_timer.function = my_hrtimer_callback;
//     hrtimer_start(&m_h->hr_timer, ktime, HRTIMER_MODE_REL );
 
//     // start = current_kernel_time(); kernel v4的写法
//     ktime_get_ts64(&start);
 
//     printk(KERN_INFO "hrtimer:%9llu sec, %9lld ns\n", 
//             (unsigned long long) start.tv_sec, (long long)start.tv_nsec ); 
 
//     return 0;
// }

// static void __exit module_hrtimer_exit( void )
// {
// 	int ret;
//   ret = hrtimer_cancel( &m_h->hr_timer);

//   if (hr_timers != NULL)
//   {
//     for (; j < size; j++)
//     {
//       ret = hrtimer_cancel(&hr_timers[j].hr_timer);
//     }
//   }

//   if (ret) 
//     printk("The timer was still in use...\n");
//   printk("Totally %d hrtimers, HR Timer module uninstalling.\n", count);
// }

// module_init(module_hrtimer_init);
// module_exit(module_hrtimer_exit);   


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/hrtimer.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/kthread.h>

#include "phy_switcher.h"
#include "vnode.h"
// #include "cJSON.h"

// #include <arch/x86/include/asm/current.h>
// #include <sys/types.h>

MODULE_LICENSE("GPL");

struct workqueue_struct *workqueue_test;

struct work{
  struct work_struct work_test;
  struct hrtimer hr_timer;
  char* data;
  char* index;
};

struct work my_work;
struct work my_work2;
struct timespec64 start;
struct timespec64 end;

unsigned long long diff_tv(struct timespec64 start, struct timespec64 end) 
{
    return (end.tv_nsec - start.tv_nsec);
}

enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{
    struct work *work_data = container_of(timer, typeof(*work_data), hr_timer);

    // printk(KERN_INFO"hrtimer:%9lu sec, %9lu ns, interval_delay=%lu us\n", 
    //         (unsigned long) uptime.tv_sec, 
    //         uptime.tv_nsec,
    //         (unsigned long)(diff_tv(uptimeLast, uptime))/1000); 

  printk(KERN_ALERT "This line is printed after 5 seconds.\n");
  printk(KERN_ALERT "And the data is: %s %s\n", work_data->data, work_data->index);

  ktime_get_ts64(&end);
  printk(KERN_INFO"hrtimer:%9llu sec, %9lld ns, interval_delay=%9llu ns\n", 
          (unsigned long long) end.tv_sec, 
          (long long)end.tv_nsec,
          (unsigned long long)(diff_tv(end, start)));
  // hrtimer_forward_now(timer, ms_to_ktime(interval));
  
  return HRTIMER_NORESTART;
}

void work_test_func(struct work_struct *work)
{
    printk("%s()\n", __func__);
    printk(KERN_ALERT "start computing\n");
    struct work *w = container_of(work, typeof(*w), work_test);
    mdelay(1000);
    w->data = "COMPLETE!";
    
    hrtimer_init(&w->hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
 
    w->hr_timer.function = my_hrtimer_callback;
    ktime_get_ts64(&start);
    hrtimer_start(&w->hr_timer, ms_to_ktime(5000*2), HRTIMER_MODE_REL );
    printk(KERN_ALERT "Computing finished!\n");
    pid_t tid = get_current()->pid;
    printk("pid=%u\n", tid);
    // printk("should stop: %d\n",kthread_should_stop());
    //queue_work(workqueue_test, &work_test);
}




static int test_init(void)
{
    printk("Hello,world!\n");

    int err = 0;
    struct vport *vp;
    struct vnode *vn;
    vp = (struct vport*)kmalloc(sizeof(struct vport), GFP_KERNEL);
    vn = (struct vnode*)kmalloc(sizeof(struct vnode), GFP_KERNEL);

    err = vnode_register(vp, vn, "eth0");
    printk("Finding netdevice %d\n", err);

    char * jsonStr = "{\"semantic\":{\"slots\":{\"name\":\"张三\"}}, \"rc\":0, \"operation\":\"CALL\", \"service\":\"telephone\", \"text\":\"打电话给张三\"}";
    cJSON * root = NULL;
    cJSON * item = NULL;//cjson对象
 
    root = cJSON_Parse(jsonStr); 
    if (!root) 
    {
        printk("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else{
        printk("%s\n", "有格式的方式打印Json:");           
        printk("%s\n\n", cJSON_Print(root));
    }

    /* 1. 自己创建一个workqueue， 中间参数为0，默认配置 */
    workqueue_test = alloc_workqueue("workqueue_test", WQ_UNBOUND, 1);
    my_work.index = "NO.1";
    my_work2.index = "NO.2";

    /* 2. 初始化一个工作项，并添加自己实现的函数 */
    INIT_WORK(&my_work.work_test, work_test_func);
    INIT_WORK(&my_work2.work_test, work_test_func);

    /* 3. 将自己的工作项添加到指定的工作队列去， 同时唤醒相应线程处理 */
    queue_work(workqueue_test, &my_work.work_test);
    queue_work(workqueue_test, &my_work2.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    // queue_work(workqueue_test, &my_work.work_test);
    
    return 0;
}

static void test_exit(void)
{
    printk("Goodbye,cruel world!\n");
    destroy_workqueue(workqueue_test);
}

module_init(test_init);
module_exit(test_exit);
MODULE_AUTHOR("Lee");
MODULE_DESCRIPTION("timer kernel module");