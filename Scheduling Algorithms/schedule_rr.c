#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "cpu.h"
#include "schedulers.h"
#include "list.h"

int task_id;
struct node *head = NULL;
struct node *copy_head;
int t_burst_t;
int shortest_time = 100000;
int complete_time;
int t_complete_time;
int waiting_time;
float avg_wait;
float avg_turna;
int count_round;
int t_response;
float avg_t_resp;
// reverse list
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
    copy_head = malloc(sizeof(struct node));
    copy_head = reverse(head);
    head = copy_head;

    while (copy_head != NULL)
    {
        count_round++;
        while (head != NULL)
        {
            if (count_round == 1)
            {
                t_response += complete_time;
            }
            if (head->task->burst > 10)
            {
                run(head->task, 10);
                head->task->burst = head->task->burst - 10;
                complete_time += 10;
            }
            else if (head->task->burst > 0 && head->task->burst <= 10)
            {
                run(head->task, head->task->burst);
                complete_time += head->task->burst;
                head->task->burst = 0;
                t_complete_time += complete_time;
                delete (&copy_head, head->task);
            }

            head = head->next;
        }
        head = copy_head;
    }
    waiting_time = t_complete_time - t_burst_t;
    avg_wait = ((float)waiting_time) / (float)task_id;
    avg_turna = ((float)t_complete_time) / (float)task_id;
    avg_t_resp = ((float)t_response / (float)task_id);
    printf("\nAverage waiting time = %.2f \n", avg_wait);
    printf("Average turnaround time = %.2f \n", avg_turna);
    printf("Average response time = %.2f \n", avg_t_resp);
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
    t_burst_t += task->burst;
    insert(&head, task);
}