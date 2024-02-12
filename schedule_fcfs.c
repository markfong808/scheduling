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
// finds the task whose name comes first in dictionary
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
        temp = temp->next; //move the current pointer
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
    printf("-------------------------------Scheduling FCFS-------------------------------\n");

    struct node *current = g_head; // pointer to traverse the task list
    int currentTime = 0;           // track the current time

    while (current != NULL)
    {
        Task *currTask = pickNextTask(); // get the next task
        run(currTask, currTask->burst); // run the task for its burst time
        currentTime += currTask->burst; // increment the current time by task's burst time
        printf("\tTime is now: %d\n", currentTime);
        current = g_head; // move to the next task
    }
}