#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "list.h"
#include "schedulers.h"
#include "task.h"

int com_time, wait_time, task_id, task_complete_time;
float avg_wait, avg_turna;

void schedule()
{

    avg_wait = ((float)wait_time) / (float)task_id;
    avg_turna = ((float)task_complete_time) / (float)task_id;

    printf("Average waiting time: %.2f\n", avg_wait);
    printf("Average turnaround time: %.2f\n", avg_turna);
    printf("Average response time: %.2f\n", avg_wait);
}

void add(char *p_name, int p_priority, int p_burst)
{
    Task task;

    task.name = p_name;
    task_id++;
    task.tid = task_id;
    task.burst = p_burst;
    task.priority = p_priority;

    com_time += p_burst;
    task_complete_time += wait_time + com_time - p_burst;

    run(&task, task.burst);
}