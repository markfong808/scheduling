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

// Function to add a new task to the list
void add(char *name, int priority, int burst)
{
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->hasRun = false;

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


bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }
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
     delete (&g_head, best_sofar);
    return best_sofar;
}

// void schedule()
// {
    
//     struct node *current = g_head;

//     int currentTime = 0; // track the current time
//     traverse(current);
    
//     while (current != NULL)
//     {
//         Task *currTask = pickNextTask();
//         printf("Picked task: [%s] [%d] [%d]\n", currTask->name, currTask->priority, currTask->burst);

//         if (currTask->burst > QUANTUM )
//         {
//             run(currTask, QUANTUM);
//             //delete (&g_head, currTask);
//             currentTime += QUANTUM;                  // update the current time by task's burst time
//             currTask->burst = currTask->burst - QUANTUM; // track the remain time by updating the current burst time
//             // printf("\tTime is now: %d\n", currentTime);           
//             // printf("\tremcainTime is now: %d\n", currTask->burst);
//             current = current->next;

//             //g_head = g_head ->next;
//         }
//         else 
//         {
//             run(currTask, currTask->burst);
//             currentTime += currTask->burst;
//             currTask->burst = currTask->burst - QUANTUM;
//             // printf("\tTime is now: %d\n", currentTime);
//             //printf("\tRemain Time is now: %d\n", currTask->burst);
//             current = current->next;
//             //current = g_head;
            
//             //delete (&g_head, currTask);
            
//             }
//             //current = current->next;
//             //current = g_head;
            
            
//     }
// //     traverse(g_head);
   
//  }
void schedule()
{

    int time = 0;
    struct node *current, *temp1;
    // outer loop resets cycle to beginning of sorted list if tasks remain after each list rotation
    while (g_head != NULL)
    {
        current = g_head;
        // inner loop cycles through list and runs tasks
        while (current != NULL)
        {
            // check if the task will be finished this round, and delete it when done.
            if (current->task->burst <= QUANTUM)
            {
                run(current->task, current->task->burst);
                time += current->task->burst;
                temp1 = current;
                current = current->next;
                delete (&g_head, temp1->task);
            }
            else
            { // else burst > quantum
                run(current->task, QUANTUM);
                time += QUANTUM;
                current->task->burst -= QUANTUM;
                current = current->next;
            }
            printf("        Time is now: %d\n", time);
        }
    }
 }