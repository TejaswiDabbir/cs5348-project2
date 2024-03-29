#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"
/* The following code is added by Tejaswi Dabbir (txd210002) 
** Accesing the new struct to get process statistics
*/
#include "pstat.h"
// End of code added

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/*
* The following code is added by Tejaswi Dabbir txd210002 and Nick Colvin nxc220016
**
** sys_settickets is a system call to set the number of lottery tickets for a process
** sys_getpinfo is a system call to return information about each process in the process table
** in the form of the struct pstat
**
*/
// sets number of tickets for a process
// returns 0 if successfull and -1 otherwise
int
sys_settickets(void)
{
  int new_tickets = 0;
  argint(0,&new_tickets);
  if (new_tickets > 0) {
    proc->tickets = new_tickets;
    return 0;
  }
  return -1;
}

int
sys_getpinfo(void)
{
  struct pstat *stats;
  if (argptr(0, (void*)&stats, sizeof(*stats)) < 0 || stats == NULL)
    return -1;
  return pinfohelper(stats);
}
/* End of code added */
