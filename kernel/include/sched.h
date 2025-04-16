#ifndef __MOS_SCHED_H_
#define __MOS_SCHED_H_

#include "os.h"
#include "task.h"
#include "utils.h"
#include "timer.h"

/* defined in entry.S */
extern void switch_to(struct context *next);
extern void schedule(void);
extern void sched_init();

#endif /* __MOS_SCHED_H__ */