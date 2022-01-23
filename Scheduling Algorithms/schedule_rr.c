#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "schedulers.h"
#include "list.h"
#include "task.h"

int task_id, waiting_time, task_completion_time, total_completion_time, shortest_time = 10000;
int total_response_time, count, total_burst_time;
float avg_waiting_time, avg_turnaround_time, avg_response_time;

struct node *head_ref = NULL;
struct node *head_copy;

struct node *reverse(struct node *p_head)
{
    struct node *r_head = NULL;

    while (p_head != NULL)
    {
        insert(&r_head, p_head->task);
        p_head = p_head->next;
    }

    return r_head;
}

void schedule()
{
    head_copy = malloc(sizeof(struct node));
    head_copy = reverse(head_ref);
    head_ref = head_copy;

    while (head_copy != NULL)
    {
        count++;
        while (head_ref != NULL)
        {
            if (count == 1)
                total_response_time += task_completion_time;

            if (head_ref->task->burst > 10)
            {
                run(head_ref->task, 10);
                head_ref->task->burst = head_ref->task->burst - 10;
                task_completion_time += 10;
            }

            else if (head_ref->task->burst > 0 && head_ref->task->burst <= 10)
            {
                run(head_ref->task, head_ref->task->burst);
                task_completion_time += head_ref->task->burst;
                head_ref->task->burst = 0;
                total_completion_time += task_completion_time;
                delete (&head_copy, head_ref->task);
            }

            head_ref = head_ref->next;
        }
        head_ref = head_copy;
    }

    waiting_time = total_completion_time - total_burst_time;
    avg_waiting_time = ((float)waiting_time) / (float)task_id;
    avg_turnaround_time = ((float)total_completion_time) / (float)task_id;
    avg_response_time = ((float)total_response_time / (float)task_id);
    printf("\nAverage waiting time = %.2f \n", avg_waiting_time);
    printf("Average turnaround time = %.2f \n", avg_turnaround_time);
    printf("Average response time = %.2f \n", avg_response_time);
}

void add(char *name, int priority, int burst)
{
    struct task *task = malloc(sizeof(struct task));
    task->name = name;
    task_id++;
    task->tid = task_id;
    task->burst = burst;
    task->priority = priority;
    // calculate total burst time
    total_burst_time += task->burst;
    insert(&head_ref, task);
}