#include "process.h"
#include <string.h>

int process_cmp(const void* a, const void* b){
	if((*(Process*)a).arrivalTime == (*(Process*)b).arrivalTime)
		return (*(Process*)a).requiredTime - (*(Process*)b).requiredTime;
	else return (*(Process*)a).arrivalTime - (*(Process*)b).arrivalTime;
}

void process_scheduling(Process* processList, int processNum, int schedulingType);

int main() {
	
	Process* processList;
	int processNum;

	char policy[64];
	int policy_type;
	
	scanf("%s", policy);
	if(strcmp(policy, "FIFO") == 0)
		policy_type = Policy_FIFO;
	else if(strcmp(policy, "SJF") == 0)
		policy_type = Policy_SJF;
	else if(strcmp(policy, "PSJF") == 0)
		policy_type = Policy_PSJF;
	else if(strcmp(policy, "RR") == 0)
		policy_type = Policy_RR;
	else {
		printf("input ERROR\n");
		return 0;
	}
	
	
	scanf("%d", &processNum);
	processList = (Process*)malloc(sizeof(Process)*processNum);
	
	// Process initilizing by input.

	for(int i = 0; i < processNum; i++) {
		scanf("%s%d%d", processList[i].name, &(processList[i].arrivalTime), &(processList[i].requiredTime));
		processList[i].inschedule = 0;
		processList[i].pid = -1;
	}
	
	// Arrange process by arrivalTime	
	qsort(processList, processNum, sizeof(Process), process_cmp);

//	for(int i=0;i<processNum;i++){
//		printf("%s %d %d\n", processList[i].name, processList[i].arrivalTime, processList[i].requiredTime);
//	}

	process_scheduling(processList, processNum, policy_type);
	return 0;
}
