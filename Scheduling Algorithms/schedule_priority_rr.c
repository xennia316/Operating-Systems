#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "schedulers.h"
#include "list.h"

int task_id, waiting_time, task_completion_time, total_completion_time, shortest_time = 10000;
int total_response_time, count, total_burst_time;
int highest_priority = -1000, last_prior = -1000;
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

int task_running(struct node *temp, Task *temp_task)
{
    while (temp != NULL)
    {
        if (temp->task == temp_task)
            return 1;
        temp = temp->next;
    }
    return 0;
}

void highest_task()
{
    struct node *same_priority = NULL;
    struct node *temp_head = head_ref;
    Task *h_task = malloc(sizeof(Task));

    int if_unique = 1;
    highest_priority = -1000;

    while (head_ref != NULL)
    {
        if (head_ref->task->priority > highest_priority)
        {
            highest_priority = head_ref->task->priority;
            h_task = head_ref->task;
        }
        head_ref = head_ref->next;
    }

    // find if task have the same priority
    head_ref = temp_head;

    while (head_ref != NULL)
    {
        if ((head_ref->task->priority == highest_priority) && (head_ref->task != h_task))
        {
            if_unique = 0;
            insert(&same_priority, head_ref->task);
        }
        if ((head_ref->task->priority == highest_priority) && (head_ref->task == h_task))
        {
            insert(&same_priority, head_ref->task);
        }
        head_ref = head_ref->next;
    }

    same_priority = reverse(same_priority);
    while (same_priority != NULL && if_unique == 0)
    {

        if (last_prior != highest_priority)
        {
            total_response_time += task_completion_time;
        }

        if (same_priority->task->burst > 10)
        {
            run(same_priority->task, 10);
            same_priority->task->burst -= 10;
            task_completion_time += 10;
        }

        else if ((same_priority->task->burst > 0) && (same_priority->task->burst <= 10))
        {
            task_completion_time += same_priority->task->burst;
            run(same_priority->task, same_priority->task->burst);
            total_completion_time += task_completion_time;

            if (task_running(head_copy, same_priority->task))
                delete (&head_copy, same_priority->task);
        }
        same_priority = same_priority->next;
    }

    if (if_unique == 1)
    {

        total_burst_time += h_task->burst;
        run(h_task, h_task->burst);
        task_completion_time += h_task->burst;
        total_response_time += task_completion_time;
        total_completion_time += task_completion_time;

        if (task_running(head_copy, h_task))
        {
            delete (&head_copy, h_task);
        }
    }
}

void schedule()
{
    head_copy = malloc(sizeof(struct node));
    head_copy = reverse(head_ref);
    head_ref = reverse(head_ref);

    while (head_copy != NULL)
    {
        highest_task();
        last_prior = highest_priority;
        head_ref = head_copy;
    }

    waiting_time = total_completion_time - total_burst_time;
    total_response_time -= total_burst_time;
    avg_waiting_time = ((float)waiting_time) / (float)task_id;
    avg_turnaround_time = ((float)total_completion_time) / (float)task_id;
    avg_response_time = ((float)total_response_time / (float)task_id);

    printf("\nAverage waiting time = %.2f \n", avg_waiting_time);
    printf("Average turnaround time = %.2f \n", avg_turnaround_time);
    printf("Average response time = %.2f \n", avg_response_time - 2 * avg_response_time);
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