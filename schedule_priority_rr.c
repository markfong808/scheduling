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

bool LexBefore(char *a, char *b)
{
    return strcmp(a, b) < 0;  
}
bool comesBefore(Task *a, Task *b)
{
    
    return a->priority > b->priority; // task with higher priority come first
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
        { // return highest priority
            best_sofar = temp->task;
        }
        if (temp->task->priority == best_sofar->priority)
        { // T4 and T5 has the same priority

            if (best_sofar->hasRun && temp->task->hasRun)
            { // if both are run,sort with lexorder
                if (temp->task->tid < best_sofar->tid)
                {

                    best_sofar = temp->task;
                }
                
            }
            else if (best_sofar->hasRun == true)
            {
                best_sofar = temp->task; // T5 is picked instead
            }
        }
        temp = temp->next; // move the current pointer
    }
    // Mark the selected task as executed
    best_sofar->hasRun = true;
    best_sofar-> tid++; // executed counter increment by 1 to track the executed time

    return best_sofar;
}

// Function to add a new task to the list
void add(char *name, int priority, int burst)
{
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->hasRun = false;
    newTask->tid = 0;
    // Check if the linked list is empty
    if (g_head == NULL)
    {
        // If the linked list is empty, set g_head to point to the new task
        insert(&g_head, newTask);
    }
    else
    {
        // If the linked list is not empty, traverse the list to find the appropriate position to insert the new task
        struct node *current = g_head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        // Insert the new task at the end of the linked list
        insert(&(current->next), newTask);
    }
}

void schedule()
{
    // traverse(g_head);
    int total_time_with_dispatcher = 0; // Total time taken by all tasks including dispatcher time
    int total_time_without_dispatcher = 0;
    int dispatcher_time = 0;
    int time = 0;
    struct node *current, *temp1, *SelectedTask;
    // outer loop resets cycle to beginning of sorted list if tasks remain after each list rotation
    while (g_head != NULL)
    {
        current = g_head; // g_head point to first node T1
        // inner loop cycles through list and runs tasks
        while (current != NULL)
        {
            SelectedTask = current;
            // find the highest priority first
            Task *currTask = pickNextTask(); // first should return T8,then T4

            // printf("Picked task: [%s] [%d] [%d]\n", currTask->name, currTask->priority, currTask->burst);
            //  Reset the execution pointer to the head of the linked list
            SelectedTask = g_head;
            while (SelectedTask != NULL)
            {

                if (SelectedTask->task == currTask)
                    current = SelectedTask;
                SelectedTask = SelectedTask->next;
            }

            if (current->task->burst <= QUANTUM)
            {
                run(current->task, current->task->burst);
                time += current->task->burst;
                total_time_with_dispatcher += current->task->burst; // accumulate total time without dispatcher
                dispatcher_time++;
                temp1 = current;
                current = current->next;
                delete (&g_head, temp1->task);
            }
            else
            { // else burst > quantum
                run(current->task, QUANTUM);
                time += QUANTUM;
                current->task->burst -= QUANTUM;
                total_time_with_dispatcher += QUANTUM;
                dispatcher_time++;
                current = current->next;
            }
            printf("        Time is now: %d\n", time);
        }
    }
    // Calculate CPU utilization
    double cpu_utilization = (double)(total_time_with_dispatcher) / (total_time_with_dispatcher + dispatcher_time - 1)  * 100;


    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}
