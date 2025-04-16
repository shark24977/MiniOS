#include "os.h"
#include "timer.h"
#include "task.h"
#include "sched.h"

struct task_control_block dummy_nodes[MAX_PRIORITY];
uint32_t ready_bitmap = 0;

void idle_task(void) {
    while (1) {
        printf("idle\n");
        static volatile int idle_count = 0;
        idle_count++;
        wait(10000);
    }
}

/* 调度器初始化 */
void sched_init() {
    for (int i = 0; i < MAX_PRIORITY; i++) {
        dummy_nodes[i].next = NULL;
        priority_array[i] = &dummy_nodes[i];
    }

    ready_bitmap = 0; 

    w_mscratch(0);
    w_mie(r_mie() | MIE_MSIE);

    task_create(idle_task, MAX_PRIORITY - 1);
}

void schedule() {
    if (ready_bitmap == 0) {  
        panic("No tasks available");
        return;
    }
    
    int highest_pri = count_trailing_zeros(ready_bitmap);
    
    struct task_control_block* dummy = priority_array[highest_pri];
    struct task_control_block* current = dummy->next;
    _current = current - tcb;

    if (current->task_status == TASK_READY)
    {
        switch_to(current->ctx);
    }   
    
}