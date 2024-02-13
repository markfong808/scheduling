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

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

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
        if (comesBefore(temp->task->name, best_sofar->name))
            best_sofar = temp->task;
        temp = temp->next; // move the current pointer
    }
    // delete the node from list, Task will get deleted later
    // delete (&g_head, best_sofar);
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

    struct node *current = g_head; // pointer to traverse the task list
    int currentTime = 0;           // track the current time
    // int remaind_time[10];

    while (current != NULL)
    {
        Task *currTask = pickNextTask();
        if (currTask->burst <= QUANTUM)
        { // condition where the CPU burst time is less than the QUANTUM, can be done in one
            run(currTask, currTask->burst);
            currentTime += currTask->burst;
            printf("\tTime is now: %d\n", currentTime);
            delete (&g_head, currTask); // delete the node if the task is finished
        }
        else if (currTask->burst > QUANTUM)
        {
            //int remain_time = 0;
            run(currTask,QUANTUM); // run the task for its burst time minus the quantum
            currentTime += QUANTUM; // update the current time by task's burst time minus the quantum
            currTask->burst = currTask->burst - QUANTUM; // track the remain time for the current task
            printf("\tTime is now: %d\n", currentTime);
            // current = current -> next;
            // while (remain_time != 0)
            // {
            //     if (remain_time <= QUANTUM)
            //     {
            //         run(currTask, remain_time);
            //         currentTime += remain_time;
            //         printf("\tTime is now: %d\n", currentTime);
            //         delete (&g_head, currTask); // delete the node if the task is finished
            //     }
            //     else
            //     {
            //         run(currTask, remain_time - QUANTUM);
            //         currentTime += remain_time - QUANTUM;
            //         printf("\tTime is now: %d\n", currentTime);
            //     }
            // }
        }
        current = g_head; // reset
    }
}