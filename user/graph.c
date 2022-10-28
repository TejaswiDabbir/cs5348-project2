/*
* The following code is added by Nick Colvin nxc220016
**
** this user program verifies the lottery scheduling policy
** by running three processes with a fixed ratio of tickets
** and prints the number of ticks each receives. It is modified
** from high_tickets.
**
*/
#include "types.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {\
   printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
   exit();}

void spin()
{
	int i = 0;
  int j = 0;
  int k = 0;
	for(i = 0; i < 5000; ++i)
	{
		for(j = 0; j < 200000; ++j)
		{
			k = j % 10;
      k = k + 1;
    }
	}
}

void print(struct pstat *st)
{
   int i;
   for(i = 0; i < NPROC; i++) {
      if (st->inuse[i]) {
          printf(1, "pid: %d tickets: %d ticks: %d\n", st->pid[i], st->tickets[i], st->ticks[i]);
      }
   }
}


int
main(int argc, char *argv[])
{
   int lowtickets = 10, medtickets = 20, hightickets = 30;
   check(settickets(lowtickets) == 0, "settickets");

   if(fork() == 0)
   {
   	    check(settickets(medtickets) == 0, "settickets");

		if (fork() == 0) {
			check(settickets(hightickets) == 0, "settickets");

			struct pstat st;
			spin();
		
			check(getpinfo(&st) == 0, "getpinfo");
			printf(1, "\n ****PInfo**** \n");
			print(&st);
		
			printf(1, "Should print 1"); 
			exit();
		}
		spin();
		printf(1, " then 2");
        while (wait() > -1);
   		exit();
   }
   spin();
   printf(1, " then 3");
   while (wait() > -1);
   exit();
}
/* End of code added */