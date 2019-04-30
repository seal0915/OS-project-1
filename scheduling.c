#include "process.h"

int specify_cpu(pid_t pid, int core_number);
int run_process(Process proc);
int set_low_priority(pid_t pid);
int set_high_priority(pid_t pid);

int start_process(Process* processList, int processNum, int nowTime){
	for(int i=0;i<processNum;i++){
		if(processList[i].arrivalTime > nowTime)return -1;
		else if(processList[i].inschedule == 0){
			processList[i].inschedule = 1;
			processList[i].pid = run_process(processList[i]);
			printf("%s %u\n", processList[i].name, processList[i].pid);
			set_low_priority(processList[i].pid);
		}
	}
}
// to replace the part that start new process
// fork a pid to process that is at ready

int processRunning;
int nowTime;
int context_switchTime;

int next_process(Process* processList, int processNum, int schedulingType){
	
	if(processRunning != -1 && (schedulingType == Policy_FIFO || schedulingType == Policy_SJF)){
		return processRunning;
	}

	int next=-1;

	if(schedulingType == Policy_SJF || schedulingType == Policy_PSJF){
		for(int i=0; i<processNum; i++){
			if(processList[i].pid == -1 || processList[i].requiredTime == 0)
				continue;
			else if(next == -1 || processList[i].requiredTime < processList[next].requiredTime)
				next = i;
		}
	}

	else if(schedulingType == Policy_FIFO){
		for(int i=0; i<processNum; i++){
			if(processList[i].pid == -1 || processList[i].requiredTime == 0)
				continue;
			else if(next == -1 || processList[i].arrivalTime < processList[next].arrivalTime)
				next = i;
		}
	}

	else if(schedulingType == Policy_RR){
		if (processRunning == -1){
			for (int i=0; i<processNum; i++){
				if(processList[i].pid != -1 && processList[i].requiredTime > 0){
					next = i;
					break;
				}
			}
		}
		else if((nowTime - context_switchTime) % 500 == 0){
			next = (processRunning + 1) % processNum;
			while(processList[next].pid == -1 || processList[next].requiredTime == 0)
				next = (next + 1) % processNum;
		}
		else next = processRunning;
	}

	return next;
}
// return the i'th process in processList by scheduling policy

void process_scheduling(Process* processList, int processNum, int schedulingType){
	
	int jobFinishedCnt = 0;
	int processNext = -1;
	processRunning = -1;
	nowTime = 0;

	specify_cpu(getpid(), CPU1);
	set_high_priority(getpid());

	while(jobFinishedCnt < processNum){
//		printf("%d, %d\n", nowTime, processRunning); // test line
		if(processRunning != -1 && processList[processRunning].requiredTime <= 0){
			waitpid(processList[processRunning].pid, NULL, 0);
//			printf("%s is back\n", processList[processRunning].name); // test line
			processRunning = -1;
			jobFinishedCnt++;

			if(jobFinishedCnt >= processNum)
				break;
			// avoid one more second waiting
		}
		start_process(processList, processNum, nowTime);


		processNext = next_process(processList, processNum, schedulingType);
		if(processNext != -1 && processRunning != processNext && processRunning != -1){
			set_high_priority(processList[processNext].pid);
			set_low_priority(processList[processRunning].pid);
//			printf("%s is up\n", processList[processNext].name); // test line
//			printf("%s is down\n", processList[processRunning].name); // test line
			processRunning = processNext;
			context_switchTime = nowTime;	
		}else if(processRunning == -1 && processNext != -1){
			set_high_priority(processList[processNext].pid);
//			printf("%s is up\n", processList[processNext].name); // test line
			processRunning = processNext;
			context_switchTime = nowTime;
		}


		run_unit_time();
		if(processRunning != -1){
			processList[processRunning].requiredTime--;
		}

		nowTime++;
		// simplify three part processing from original code
	}
}