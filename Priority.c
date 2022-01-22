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
	printf("\nEnter process priority:");
	scanf("%d",&p.priority);
	return p;
}
int main(){
	int n, i = 0, j = 0, turnArrT, waitTime = 0,totalWaitingTime = 0, totalTurnArrTime;
	float avgWaitingTime, avgTurnArrTime;
	
	printf("\nenter the number of processes:");
	scanf("%d", &n);
	
	struct process proc[n];
	int waitingTime[n], turnAT[n];

	//getting the processes into an array
	for(i = 0; i<n; i++)
		proc[i] = getProcesses(i+1); 
	
	for(i = 0; i<n-1; i++){
		for(j = i+1; j<n; j++){
			if(proc[i].priority < proc[j].priority){
				struct process temp;
				temp = proc[i];
				proc[i] = proc[j];
				proc[j] = temp;
			}
		}
	}
	
	waitingTime[0] = 0;
	
	for(i = 1; i<n; i++){
		waitTime += proc[i-1].burst_time;
		proc[i].waiting_time = waitTime;
		totalWaitingTime += waitTime;
	}
	
	printf("order of execution:\n");
	
	for(i = 0; i<n; i++){
		turnArrT = proc[i].waiting_time + proc[i].burst_time;
		totalTurnArrTime += turnArrT;
		proc[i].turnAT = turnArrT;
		printf("\t\tprocess[%d]\n", proc[i].name);
	}
	printf("\n");
	avgWaitingTime = (totalWaitingTime/n);
	avgTurnArrTime = (totalTurnArrTime/n);
	
	printf("Process\t\tburst time\twaiting time\tturn around time\n");

	for(int i = 0; i<n; i++){
		printf("Process[%d]\t\t%d\t\t%d\t\t%d\n", proc[i].name,  proc[i].burst_time,proc[i].waiting_time,proc[i].turnAT);
	}
	printf("\n");
	printf("Average turn arround time: %5.2f\nAverage waiting time: %5.2f\n", avgTurnArrTime, avgWaitingTime);

	return 0;
}