#include<stdio.h>
#include<stdlib.h>

struct process{
	int name;
	int burst_time;
	int priority;
	int waiting_time;
	int turnAT;
};

struct process getProcesses(int i){
	struct process p;
	p.name = i;
	printf("Enter process [%d] information:", i);
	printf("\nEnter process burst time:");
	scanf("%d",&p.burst_time);
	return p;
}
int main(){
	int n, turnArrT, waitTime = 0,totalWaitingTime = 0, totalTurnArrTime;
	float avgWaitingTime, avgTurnArrTime;
	printf("\nenter the number of processes:");
	scanf("%d", &n);
	struct process proc[n];
	int waitingTime[n], turnAT[n];
	//getting the processes into an array
	for(int i = 0; i<n; i++)
		proc[i] = getProcesses(i+1); 
		
	waitingTime[0] = 0;
	for(int i = 1; i<n; i++){
		waitTime += proc[i-1].burst_time;
		waitingTime[i] = waitTime;
		totalWaitingTime += waitTime;
	}
	
	for(int i = 0; i<n; i++){
		turnArrT = waitingTime[i] + proc[i].burst_time;
		totalTurnArrTime += turnArrT;
		turnAT[i] = turnArrT;
	}
	avgWaitingTime = (totalWaitingTime/n);
	avgTurnArrTime = (totalTurnArrTime/n);
		printf("\n");
	printf("Process\t\tburst time\twaiting time\tturn around time\n");

	for(int i = 0; i<n; i++){
		printf("process[%d]\t\t%d\t\t%d\t\t%d\n", proc[i].name,  proc[i].burst_time,waitingTime[i],turnAT[i]);
	}
	printf("\n");
	printf("average turn arround time: %5.2f\naverage waiting time: %5.2f", avgTurnArrTime, avgWaitingTime);

	return 0;
}
