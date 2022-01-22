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

struct process getProcesses(int process_id);

int main() {
	int number_of_processes, i = 0,t, j = 0, total_time = 0, turnArrT = 0, quantum, counter, waitTime = 0,totalWaitingTime = 0, totalTurnArrTime;
    int status;
	float avgWaitingTime, avgTurnArrTime;
	
	printf("\nEnter the number of processes:");
	scanf("%d", &number_of_processes);

    printf("Enter the quantum:");
    scanf("%d",& quantum);
	
	struct process proc[number_of_processes];
	int waitingTime[number_of_processes], turnAT[number_of_processes];

	//getting the processes into an array
	for (i = 0; i < number_of_processes; i++)
		proc[i] = getProcesses(i+1); 
    
    i = 0;
    do {
        if (proc[i].rem_BT != 0) {
            if (proc[i].rem_BT > quantum) {
                proc[i].rem_BT -= quantum;
                total_time += quantum;
            }
            else {
                total_time += proc[i].rem_BT;
            }
            status = 0;
        }
        else if (proc[i].rem_BT == 0 && status != 0) {
            status = 1;
        }
        i++;
        if (i > number_of_processes) {
            i = 0;
        }
    } while(!status);

    printf("Process\t\tburst time\twaiting time\tturn around time\n");

	for(int i = 0; i < number_of_processes; i++){
		printf("Process[%d]\t\t%d\t\t%d\t\t%d\n", proc[i].name,  proc[i].burst_time,proc[i].waiting_time,proc[i].turnAT);
	}
	printf("\n");
	printf("Average turn arround time: %5.2f\nAverage waiting time: %5.2f\n", avgTurnArrTime, avgWaitingTime);

}
    

struct process getProcesses(int process_id) {
	struct process p;
	p.name = process_id;
	printf("\n\t\tEnter process [%d] information\n", process_id);
	printf("Enter process burst time: ");
	scanf("%d", &p.burst_time);
	printf("Enter process priority: ");
	scanf("%d", &p.priority);
	return p;
}
