#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct process{
	int name;
	int burst_time;
	int priority;
	int waiting_time;
	int turnAT;
    int rem_BT;
};

struct process getProcesses(int i);

int main() {
	int n, i = 0,t, j = 0, total, turnArrT = 0, quantum, counter, waitTime = 0,totalWaitingTime = 0, totalTurnArrTime;
	float avgWaitingTime, avgTurnArrTime;
	
	printf("\nenter the number of processes:");
	scanf("%d", &n);

    printf("Enter the quantum:");
    scanf("%d",& quantum);
	
	struct process proc[n];
	int waitingTime[n], turnAT[n];

	//getting the processes into an array
	for (i = 0; i < n; i++)
		proc[i] = getProcesses(i+1); 
    
    for (total = 0, i = 0; n != 0;) {
        // printf("1. counter: %d, burst time: %d\n", counter, proc[i].burst_time); 
        if(proc[i].burst_time <= quantum && proc[i].burst_time > 0) 
        { 
            printf("n: %d\n", n);
            total = total + proc[i].burst_time; 
            proc[i].burst_time = 0; 
            counter = 1; 
            // printf("2.counter: %d, burst time: %d\n", counter, proc[i].burst_time);
        } 
        else if(proc[i].burst_time > 0) 
        { 
            printf("burst time: %d\n", total);
            proc[i].burst_time = proc[i].burst_time - quantum; 
            total = total + quantum; 
        }
        printf("3.counter: %d, burst time: %d\n", counter, proc[i].burst_time);
        if(counter == 1 && proc[i].burst_time == 0) 
        { 
                n--; 
                printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", i + 1, proc[i].burst_time, total - 0, total - 0 - proc[i].burst_time);
                waitTime= waitTime+ total - 0 - 
                proc[i].burst_time; 
                proc[i].waiting_time = waitTime;
                turnArrT= turnArrT+ total - 0; 
                counter = 0; 
        } 
        if(i == n - 1) 
        {
                i = 0; 
        }
        else 
        {
                i = 0;
        }
        // printf("%d\n", n);
    } 

    printf("Process\t\tburst time\twaiting time\tturn around time\n");

	for(int i = 0; i<n; i++){
		printf("Process[%d]\t\t%d\t\t%d\t\t%d\n", proc[i].name,  proc[i].burst_time,proc[i].waiting_time,proc[i].turnAT);
	}
	printf("\n");
	printf("Average turn arround time: %5.2f\nAverage waiting time: %5.2f\n", avgTurnArrTime, avgWaitingTime);

}
    

struct process getProcesses(int i) {
	struct process p;
	p.name = i;
	printf("Enter process [%d] information:", i);
	printf("\nEnter process burst time:");
	scanf("%d",&p.burst_time);
	printf("\nEnter process priority:");
	scanf("%d",&p.priority);
	return p;
}