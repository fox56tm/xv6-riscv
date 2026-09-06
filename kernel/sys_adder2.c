#include "kernel/types.h"
#include "kernel/stat.h"
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "syscall.h"
#include "defs.h"

uint64 sys_adder2(void){

    printk("=== adder2 syscall was called\n");
    int a,b;
    
    argint(0, &a);
    argint(1,&b);
    printk("=== arg a: %d, arg b: %d\n", a,b);
    return a + b;
}
