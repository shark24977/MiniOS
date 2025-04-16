#include "os.h"
#include "timer.h"
#include "task.h"

/* 全局定时器链表头，链表内的定时器按照 timeout_tick 递增排序 */
static struct timer *timer_list_head = NULL;
static uint32_t _tick = 0;

void timer_list_insert(struct timer **head, struct timer *new_timer)
{
    if (*head == NULL || new_timer->timeout_tick < (*head)->timeout_tick) {
        new_timer->next = *head;
        *head = new_timer;
    } else {
        struct timer *p = *head;
        while (p->next && p->next->timeout_tick <= new_timer->timeout_tick) {
            p = p->next;
        }
        new_timer->next = p->next;
        p->next = new_timer;
    }
}


void timer_list_remove(struct timer **head, struct timer *target)
{
    if (*head == target) {
        *head = (*head)->next;
        return;
    }
    struct timer *p = *head;
    while (p && p->next != target) {
        p = p->next;
    }
    if (p && p->next == target) {
        p->next = target->next;
    }
}


struct timer *timer_list_pop(struct timer **head)
{
    if (*head == NULL)
        return NULL;
    struct timer *t = *head;
    *head = (*head)->next;
    t->next = NULL;
    return t;
}


void timer_load(int interval)
{
    
    int id = r_mhartid();
    *(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}


void timer_init()
{
    timer_load(TIMER_INTERVAL);
    w_mie(r_mie() | MIE_MTIE);
    timer_list_head = NULL;
}

struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
    if (handler == NULL || timeout == 0) {
        return NULL;
    }

    struct timer *new_timer = (struct timer *)malloc(sizeof(struct timer));
    if (!new_timer) {
        return NULL;
    }
    new_timer->func = handler;
    new_timer->arg = arg;
    new_timer->timeout_tick = _tick + timeout;
    new_timer->next = NULL;

    timer_list_insert(&timer_list_head, new_timer);

    return new_timer;
}

void timer_delete(struct timer *timer)
{
    if (timer == NULL)
        return;
    timer_list_remove(&timer_list_head, timer);
    free(timer);
}


static inline void timer_check()
{
    
    if (timer_list_head == NULL || _tick < timer_list_head->timeout_tick) {            
        return;
    }
    struct timer *t = timer_list_pop(&timer_list_head);
    t->func(t->arg);
    free(t);
    
}

void timer_handler() {

	_tick++;

	timer_check();

    timer_load(TIMER_INTERVAL);
    
    struct task_control_block *current_task = &tcb[_current];
    current_task->time_slice--;
        if (current_task->time_slice <= 0) {
            int id = r_mhartid();
            *(volatile uint32_t*)CLINT_MSIP(id) = 1;
        }
}