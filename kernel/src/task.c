#include "os.h"
#include "task.h"
#include "utils.h"
#include "timer.h"

#define STACK_SIZE 10240
#define TIME_SLICE 3

// 全局静态分配的任务堆栈、上下文和 TCB 数组
uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];

struct task_control_block* priority_array[MAX_PRIORITY] = {0};
struct task_control_block tcb[MAX_TASKS] = {0};
struct context ctx_tasks[MAX_TASKS] = {0};
struct task_control_block *blocked_queue = NULL;

int _top = 0;
int _current = -1;


int task_create(void (*start_routin)(void), int pri) {
    if (pri < 0 || pri >= MAX_PRIORITY) {
        printf("Invalid priority: %d\n", pri);
        return -1;
    }

    int task_id = _top;
    tcb[task_id].taskID = task_id;
    tcb[task_id].ctx = &ctx_tasks[task_id];
    tcb[task_id].ctx->sp = (reg_t)&task_stack[task_id][STACK_SIZE];
    tcb[task_id].ctx->pc = (reg_t) start_routin; 
    tcb[task_id].task_status = TASK_READY; 
    tcb[task_id].priority = pri;
    tcb[task_id].time_slice = TIME_SLICE;
    tcb[task_id].next = NULL;
    tcb[task_id].WaitNext = NULL;

    list_insert_tail(priority_array[pri], &tcb[task_id]);
    _top++;
    
    update_ready_bit(pri);

    return 0;
}


void task_exit() {
    struct task_control_block* current_task = &tcb[_current];
    int pri = current_task->priority;
    struct task_control_block* dummy = priority_array[pri];
    if (!dummy) {
        printf("Error: Dummy task control block is NULL\n");
        return;
    }

    current_task->task_status = TASK_TERMINATED;
    list_remove(dummy, current_task);

    update_ready_bit(pri);
    
    int id = r_mhartid();
    *(uint32_t*)CLINT_MSIP(id) = 1;
}


void wakeup_task(void *arg) {
    
    printf("======> delay finish\n");
    if (arg == NULL)
        return;
    struct task_control_block *task = (struct task_control_block *)arg;
    int pri = task->priority;

    remove_from_blocked_queue(task);
    
    task->task_status = TASK_READY;
    
    list_insert_tail(priority_array[pri], task);
    update_ready_bit(pri);
    schedule();
}

/* tick 单位为系统 TICK */
int task_delay(uint32_t tick) {
    
    int curr = _current;
    struct task_control_block *current_task = &tcb[curr];
    int pri = current_task->priority;

    list_remove(priority_array[pri], current_task);
    update_ready_bit(pri);

    current_task->task_status = TASK_SUSPENDED;

    add_to_blocked_queue(current_task);

    struct timer *tm = timer_create(wakeup_task, current_task, tick);
    if (NULL == tm) {
		printf("timer_create() failed!\n");
	}
    
    task_yield();

    return 0;
}

/* 显式让出 CPU */
void task_yield() {

    int id = r_mhartid();
    *(uint32_t*)CLINT_MSIP(id) = 1;
}

void wait(volatile int count)
{
	count *= 50000;
	while (count--);
}