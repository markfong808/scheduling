#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Global variable for the head of the linked list
struct node *g_head = NULL;

bool comesBefore(Task *a, Task *b)
{
    if (a->priority == b->priority) // in case of a tie in prioriy order, tasks with lexicographically smaller names being executed first
    {
        return strcmp(a->name, b->name) < 0;
    }
    else
    {
        return a->priority > b->priority; // task with higher priority come first
    }
}

// based on traverse from list.c
// finds the task who has the shortest CPU brust time
Task *pickNextTask()
{
    // if list is empty, nothing to do
    if (!g_head)
        return NULL;

    struct node *temp;
    temp = g_head;
    Task *best_sofar = temp->task;

    while (temp != NULL)
    {
        if (comesBefore(temp->task, best_sofar))
            best_sofar = temp->task;
        temp = temp->next;
    }
    // delete the node from list, Task will get deleted later
    delete (&g_head, best_sofar);
    return best_sofar;
}

// Function to add a new task to the list
void add(char *name, int priority, int burst)
{
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    insert(&g_head, newTask);
}

void schedule()
{
    int total_time_with_dispatcher = 0; // Total time taken by all tasks including dispatcher time
    int total_time_without_dispatcher = 0;
    struct node *current = g_head; // pointer to traverse the task list
    int currentTime = 0;           // track the current time
    int dispatcher_time = 0;      // Dispatcher time is 1 unit

    while (current != NULL)
    {
        Task *currTask = pickNextTask();
        run(currTask, currTask->burst); // run the task for its burst time
        currentTime += currTask->burst; // increment the current time by task's burst time
        printf("\tTime is now: %d\n", currentTime);
        total_time_with_dispatcher += currTask->burst; // accumulate total time with dispatcher
        current = g_head; // reset the current pointer, g_head never moved
        dispatcher_time++;

    }
    // Calculate CPU utilization
    double cpu_utilization = (double)(total_time_with_dispatcher) / (total_time_with_dispatcher + dispatcher_time - 1)  * 100;

    printf("CPU Utilization: %.3f%%\n", cpu_utilization);
}