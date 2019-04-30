#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>


#define run_unit_time()		        	\
{										\
	volatile unsigned long i;			\
	for (i = 0; i < 1000000UL; i++);	\
}										\
// Macro for unit time running
 
#define CPU1 0 //Parent
#define CPU2 1 //Child	

#define Policy_FIFO 1
#define Policy_SJF 2
#define Policy_PSJF 3
#define Policy_RR 4



typedef struct Process {
    char name[32];
    int arrivalTime;
    int requiredTime;
   	int inschedule;
    pid_t pid;
}Process;


/*
	Lee : Should library declare each file? 
*/