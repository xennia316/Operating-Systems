#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./cpu.h"
#include "schedulers.h"
#include "list.h"
#include "task.h"

int task_id, waiting_time, task_completion_time, total_completion_time, shortest_time = 1000;
float avg_waiting_time, avg_turnaround_time;

struct node *head_ref = NULL, *head_copy;

void schedule()
{
    head_copy = malloc(sizeof(struct node));

    head_copy = head_ref;
    Task *shortest_task = malloc(100);

    while (head_ref != NULL)
    {
        while (head_ref != NULL)
        {
            if (head_ref->task->burst <= shortest_time)
            {
                shortest_time = head_ref->task->burst;
                memcpy(shortest_task, head_ref->task, 100);
            }
            head_ref = head_ref->next;
        }

        run(shortest_task, shortest_task->burst);
        task_completion_time += shortest_task->burst;
        total_completion_time += task_completion_time;
        waiting_time = waiting_time + task_completion_time - shortest_task->burst;

        delete (&head_copy, shortest_task);

        head_ref = head_copy;
        shortest_time = 1000;
    }

    avg_waiting_time = ((float)waiting_time) / (float)task_id;
    avg_turnaround_time = ((float)total_completion_time) / (float)task_id;

    printf("\nAverage waiting time = %.2f \n", avg_waiting_time);
    printf("Average turnaround time = %.2f \n", avg_turnaround_time);
    printf("Average response time = %.2f \n", avg_waiting_time);
}

void add(char *name, int p_priority, int p_burst)
{
    Task *process;
    process = malloc(sizeof(Task));

    process->name = name;
    task_id++;
    process->tid = task_id;
    process->burst = p_burst;
    process->priority = p_priority;

    insert(&head_ref, process);
}