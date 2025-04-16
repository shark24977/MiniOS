#include "os.h"
#include "timer.h"
#include "task.h"
#include "sched.h"

#define DELAY 1000

void user_task0(void)
{
	
	task_delay(10);
	while (1)
	{
		printf("Task 0: Running...\n");
		wait(DELAY);
	}	
	
}

void user_task1(void)
{
	task_delay(5);
	while (1)
	{
		printf("Task 1: Running...\n");
		wait(DELAY);
	}
	
}

/* NOTICE: DON'T LOOP INFINITELY IN main() */
void os_main(void)
{	
	task_create(user_task0,2);
	task_create(user_task1,2);
}

