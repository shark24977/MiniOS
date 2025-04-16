#include "os.h"
#include "utils.h"

void update_ready_bit(int pri) {
    struct task_control_block* dummy = priority_array[pri];
    if (dummy->next == NULL) {
        clear_ready(pri);
    } else {
        set_ready(pri);
    }
}

void list_remove(struct task_control_block *head, struct task_control_block *node) {
    struct task_control_block *prev = head;
    struct task_control_block *curr = head->next;
    while (curr != NULL) {
        if (curr == node) {
            prev->next = curr->next;
            curr->next = NULL;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void list_insert_tail(struct task_control_block *head, struct task_control_block *node) {
    struct task_control_block *iter = head;
    while (iter->next != NULL) {
        iter = iter->next;
    }
    iter->next = node;
    node->next = NULL;
}

// 将任务加入阻塞队列（简单链表头插）
void add_to_blocked_queue(struct task_control_block *task) {
    task->WaitNext = blocked_queue;
    blocked_queue = task;
}

// 从阻塞队列中移除任务（简单查找）
void remove_from_blocked_queue(struct task_control_block *task) {
    if (blocked_queue == task) {
        blocked_queue = task->WaitNext;
        task->WaitNext = NULL;
        return;
    }
    struct task_control_block *prev = blocked_queue;
    while (prev && prev->WaitNext != task) {
        prev = prev->WaitNext;
    }
    if (prev)
        prev->WaitNext = task->WaitNext;
    task->WaitNext = NULL;
}
