#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "riscv.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>


/* uart */
extern int uart_putc(char ch);
extern void uart_puts(char *s);
extern int uart_getc(void);

/* printf */
extern int  printf(const char* s, ...);
extern void panic(char *s);

/* plic */
extern int plic_claim(void);
extern void plic_complete(int irq);

/* interrupts Lock */
extern int ArchIntLock();
extern int ArchIntRestore( int intSave);

/* mem */
extern void *malloc(int size);
extern void free(void *p);


#endif /* __OS_H__ */
