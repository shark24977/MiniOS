#include "os.h"
#include "task.h"
#include "sched.h"
#include "utils.h"

extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_handler(void);
extern void schedule(void);

void trap_init()
{
	/*
	 * set the trap-vector base-address for machine-mode
	 */
	w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ){
      		uart_isr();
	} else if (irq) {
		printf("unexpected interrupt irq = %d\n", irq);
	}
	
	if (irq) {
		plic_complete(irq);
	}
}


void software_interrupt_handler() {

    int id = r_mhartid();
    *(volatile uint32_t*)CLINT_MSIP(id) = 0;
    
    int pri = tcb[_current].priority;
    struct task_control_block *dummy = priority_array[pri];
    struct task_control_block *current_task = &tcb[_current];

	if (current_task->task_status == TASK_TERMINATED || current_task->task_status == TASK_SUSPENDED) {
        schedule();
		return;
    }
            
    if (current_task->next != NULL) {
            
        struct task_control_block *next_task = current_task->next;
        list_remove(dummy, current_task);
        list_insert_tail(dummy, current_task);
        _current = next_task - tcb;
        switch_to(next_task->ctx);
    } 
            
}


reg_t trap_handler(reg_t epc, reg_t cause)
{
	reg_t return_pc = epc;
	reg_t cause_code = cause & MCAUSE_MASK_ECODE;
	
	if (cause & MCAUSE_MASK_INTERRUPT) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			//uart_puts("software interruption!\n");
			/*
			 * acknowledge the software interrupt by clearing
    			 * the MSIP bit in mip.
			 */
			software_interrupt_handler();
			break;
		case 7:
			//uart_puts("timer interruption!\n");
			timer_handler();
			break;
		case 11:
			uart_puts("external interruption!\n");
			external_interrupt_handler();
			break;
		default:
			printf("Unknown async exception! Code = %ld\n", cause_code);
			break;
		}
	} else {
		/* Synchronous trap - exception */
		printf("Sync exceptions! Code = %ld\n", cause_code);
		panic("OOPS! What can I do!");
		//return_pc += 4;
	}

	return return_pc;
}

void trap_test()
{
	/*
	 * Synchronous exception code = 7
	 * Store/AMO access fault
	 */
	*(int *)0x00000000 = 100;

	/*
	 * Synchronous exception code = 5
	 * Load access fault
	 */
	//int a = *(int *)0x00000000;

	uart_puts("Yeah! I'm return back from trap!\n");
}

