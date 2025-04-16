#ifndef __MOS_TASK_H__
#define __MOS_TASK_H__

#include "types.h"


#define MAX_TASKS 20
#define MAX_PRIORITY 31


struct context {
	/* ignore x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;
	// upon is trap frame

	// save the pc to run in next schedule cycle
	reg_t pc; // offset: 31 * sizeof(reg_t)
};

typedef enum {
    TASK_READY = 0,        // 任务就绪
    TASK_SUSPENDED = 1,    // 任务挂起
    TASK_TERMINATED = 2,   // 任务终止
} task_status_t;

// 任务控制块结构体
struct task_control_block {
    int taskID;
    task_status_t task_status;           
    int priority;              
    struct context *ctx;       
    struct task_control_block *next;  
    struct task_control_block *WaitNext; 
    int time_slice;
};

extern int _top;
extern int _current;

extern struct task_control_block* priority_array[MAX_PRIORITY];
extern struct task_control_block tcb[MAX_TASKS];
extern struct context ctx_tasks[MAX_TASKS];
extern struct task_control_block *blocked_queue;


extern void switch_to(struct context *next);
extern void schedule(void);

extern int  task_create(void (*task)(void), int pri);
extern int  task_delay(uint32_t tick);
extern void task_yield();
extern void task_exit(void);
extern void wait(volatile int count);

#endif /* __MOS_TASK_H__ */
