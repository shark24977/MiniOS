#ifndef __MOS_TIMER_H__
#define __MOS_TIMER_H__

#include "os.h"

// 将定时器中断间隔设置为更合理的值（例如每秒100次中断）
#define CLINT_TIMEBASE_FREQ 10000000     // 假设时钟频率为10MHz
#define TIMER_INTERVAL (CLINT_TIMEBASE_FREQ / 10)  // 每秒100次中断（即10ms一次）

/* 定时器结构体*/
struct timer { 
    void (*func)(void *arg);
    void *arg;
    uint32_t timeout_tick;
    struct timer *next;  
};


// 定时器管理
extern void timer_load(int interval);
extern struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout);
extern void timer_delete(struct timer *timer);
extern void timer_list_insert(struct timer **head, struct timer *new_timer);
extern void timer_list_remove(struct timer **head, struct timer *target);
extern struct timer *timer_list_pop(struct timer **head);

#endif /* __MOS_TIMER_H__ */
