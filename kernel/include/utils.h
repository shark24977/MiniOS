#ifndef UTILS_H
#define UTILS_H

#include "os.h"
#include "task.h"

extern uint32_t ready_bitmap;

static inline int count_trailing_zeros(uint32_t x) {
    if (x == 0) return 32;
    int n = 0;
    while ((x & 1) == 0) {
        n++;
        x >>= 1;
    }
    return n;
}

static inline void set_ready(int pri) {
    ready_bitmap |= (1U << pri);
}

static inline void clear_ready(int pri) {
    ready_bitmap &= ~(1U << pri);
}

extern void update_ready_bit(int pri);

extern void list_remove(struct task_control_block *head, struct task_control_block *node);
extern void list_insert_tail(struct task_control_block *head, struct task_control_block *node);

extern void add_to_blocked_queue(struct task_control_block *task);
extern void remove_from_blocked_queue(struct task_control_block *task);



#endif /* __UTILS_H */