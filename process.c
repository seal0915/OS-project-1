#define _GNU_SOURCE
#include "process.h"

#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define HAPPYPRINT 400
int specify_cpu(pid_t pid, int core_number) {
	int cpu_num = sizeof(cpu_set_t);
	if(core_number > cpu_num){
		printf("specify_cpu error!\n");
		return -1;
	}
		
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core_number, &mask);

	sched_setaffinity(pid, sizeof(mask), &mask);
	return 1;
}

int run_process(Process proc){
//	printf("try to run process %s\n", proc.name);
	pid_t pid = fork();
	if(pid < 0){
		printf("fork error\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid == 0) {
		struct timeval startTime, endTime;

		gettimeofday(&startTime, NULL);

		for(int i = 0; i < proc.requiredTime; i++) {
			run_unit_time();
		}

		gettimeofday(&endTime, NULL);

		long long st = startTime.tv_sec*1000000 + startTime.tv_usec;
		long long ed = endTime.tv_sec*1000000 + endTime.tv_usec;

// can run with root, basically write directly into dmesg's buffer

		char cmd[1024];
		sprintf(cmd, "echo \"2[Project1] %u %lu.%lu %lu.%lu\" > /dev/kmsg", getpid(),
		 startTime.tv_sec, startTime.tv_usec, endTime.tv_sec, endTime.tv_usec);
		system(cmd);
//		printf("%s\n", cmd);


//	Plan B
//		sprintf(cmd, "[Project1] %u %lu.%lu %lu.%lu", getpid(),
//		 startTime.tv_sec, startTime.tv_usec, endTime.tv_sec, endTime.tv_usec);
//		syscall(HAPPYPRINT, cmd);

/*
	write a system call, register in number 400 that printk something.


*/


		exit(0);
	}
	specify_cpu(pid, CPU2);
	return pid;
}

int set_low_priority(pid_t pid) {
	struct sched_param param;
	param.sched_priority = 0;

	int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
	return ret;
}
int set_high_priority(pid_t pid) {
	struct sched_param param;
	param.sched_priority = 0;

	int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
	
	return ret;
}
