#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

int task_id, task_completion_time, waiting_time, total_completion_time;
int h_priority = -10;
float avg_waiting_time, avg_turnaround_time;

struct node *head_ref = NULL;
struct node *head_copy;

void schedule()
{
    head_copy = malloc(sizeof(struct node));
    head_copy = head_ref;

    Task *highest_priority_task = malloc(sizeof(Task));

    while (head_ref != NULL)
    {
        while (head_ref != NULL)
        {
            if (head_ref->task->priority >= h_priority)
            {
                h_priority = head_ref->task->priority;
                highest_priority_task = head_ref->task;
            }
            head_ref = head_ref->next;
        }

        run(highest_priority_task, highest_priority_task->burst);
        task_completion_time += highest_priority_task->burst;
        total_completion_time += task_completion_time;
        waiting_time = waiting_time + task_completion_time - highest_priority_task->burst;
        delete (&head_copy, highest_priority_task);
        head_ref = head_copy;
        h_priority = -10;
    }
    avg_waiting_time = ((float)waiting_time) / (float)task_id;
    avg_turnaround_time = ((float)total_completion_time) / (float)task_id;
    printf("\nAverage waiting time = %.2f \n", avg_waiting_time);
    printf("Average turnaround time = %.2f \n", avg_turnaround_time);
    printf("Average response time = %.2f \n", avg_waiting_time);
}

void add(char *name, int priority, int burst)
{
    struct task *task = malloc(sizeof(struct task));
    task->name = name;
    task_id++;
    task->tid = task_id;
    task->burst = burst;
    task->priority = priority;

    insert(&head_ref, task);
}